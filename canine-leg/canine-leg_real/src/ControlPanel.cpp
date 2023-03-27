//
// Created by hs on 22. 10. 27.
//

#include <canine-leg_real/ControlPanel.hpp>

extern pUI_COMMAND sharedCommand;
extern pSHM sharedMemory;

ControlPanel::ControlPanel(raisim::World* world, raisim::ArticulatedSystem* robot)
    : mWorld(world)
    , mRobot(robot)
    , mIteration(0)
    , mMotionTableIdx(0)
    , mReferenceTime(0.0)
{
    mTorque.setZero();
    clearBuffer();
}

void ControlPanel::ControllerFunction()
{
    sharedMemory->localTime = mIteration * CONTROL_dT;
    mIteration++;
    switch (sharedMemory->controlState)
    {
    case STATE_CONTROL_STOP:
    {
        break;
    }
    case STATE_HOME_READY:
    {
        updateStates();
        PDcontrol.InitHomeTrajectory();
        sharedMemory->controlState = STATE_HOME_CONTROL;
        sharedMemory->visualState = VISUAL_UPDATE_RAISIM;
        break;
    }
    case STATE_HOME_CONTROL:
    {
        PDcontrol.DoHomeControl();
        break;
    }
    case STATE_CUBIC_READY:
    {
        sharedMemory->cubicGoalHeight = sharedMemory->motionTableOffset[mMotionTableIdx];
        sharedMemory->cosAmplitude = sharedMemory->motionTableAmplitude[mMotionTableIdx];
        sharedMemory->cosFrequency = sharedMemory->motionTableFrequency[mMotionTableIdx];
        mMotionTableIdx++;
        std::cout << "[SYSTEM] offset, amplitude, frequency : " << sharedMemory->cubicGoalHeight << ", " << sharedMemory->cosAmplitude << ", " << sharedMemory->cosFrequency << std::endl;\
        PDcontrol.InitCubicTrajectory();
        mReferenceTime = sharedMemory->localTime;
        sharedMemory->dataCollectStopFlag = false;
        sharedMemory->controlState = STATE_CUBIC_CONTROL;
        break;
    }
    case STATE_CUBIC_CONTROL:
    {
        PDcontrol.DoCubicControl();
        if(sharedMemory->localTime >= mReferenceTime + 2.0)
        {
            sharedMemory->controlState = STATE_COS_READY;
        }
        break;
    }
    case STATE_COS_READY:
    {
        PDcontrol.InitCosTrajectory();
        mReferenceTime = sharedMemory->localTime;
        sharedMemory->controlState = STATE_COS_CONTROL;
        break;
    }
    case STATE_COS_CONTROL:
    {
        PDcontrol.DoCosControl();
        if(sharedMemory->dataCollectStopFlag)
        {
            sharedMemory->controlState = STATE_CUBIC_READY;
            if(mMotionTableIdx == 27)
            {
                std::cout << "[SYSTEM] Collecting data is end. " << std::endl;
                sharedMemory->controlState = STATE_HOME_READY;
            }
        }
        break;
    }
    default:
        break;
    }
    if(sharedMemory->visualState == VISUAL_UPDATE_RAISIM)
    {
        integrateSimul();
    }
//    if (sharedMemory->measuredGRF < 0.1)
//    {
//        sharedMemory->estimatedGRFMLP = 0.0;
//    }
}

void ControlPanel::integrateSimul()
{
    for (int idx = 0; idx < MOTOR_NUM; idx++)
    {
        mTorque[idx + 1] = sharedMemory->motorDesiredTorque[idx];
    }
    mRobot->setGeneralizedForce(mTorque);
    mWorld->integrate();
    updateStates();
    updateBuffer();
    updateNetInputs();
}

void ControlPanel::updateStates()
{
    const double mean = 0.0;
    const double stddev = 0.075;
    std::mt19937 generator(std::random_device{}());
    std::normal_distribution<double> dist_vel(mean, stddev);
    sharedMemory->motorPosition[0] = mRobot->getGeneralizedCoordinate()[1];
    sharedMemory->motorPosition[1] = mRobot->getGeneralizedCoordinate()[2];
    sharedMemory->hipVerticalVelocity = mRobot->getGeneralizedVelocity()[0];
//    sharedMemory->motorVelocity[0] = mRobot->getGeneralizedVelocity()[1] + dist_vel(generator);
//    sharedMemory->motorVelocity[1] = mRobot->getGeneralizedVelocity()[2] + dist_vel(generator);
    sharedMemory->motorVelocity[0] = mRobot->getGeneralizedVelocity()[1];
    sharedMemory->motorVelocity[1] = mRobot->getGeneralizedVelocity()[2];
    sharedMemory->hipVerticalPosition = 0.23 * cos (sharedMemory->motorPosition[0]) + 0.23 * cos (sharedMemory->motorPosition[0] + sharedMemory->motorPosition[1]);
    sharedMemory->motorTorque[0] = sharedMemory->motorDesiredTorque[0];
    sharedMemory->motorTorque[1] = sharedMemory->motorDesiredTorque[1];

    if (&(mRobot->getContacts()[0]) != nullptr)
    {
        sharedMemory->measuredGRF = mRobot->getContacts()[0].getImpulse()[2] / CONTROL_dT;
    }
}

void ControlPanel::clearBuffer()
{
    for (int i = 0; i < NET_INPUT_BUFFER_SIZE ; i++)
    {
        for (int j = 0; j < 2 ; j++)
        {
            mBufferPosition[i][j] = 0.0;
            mBufferVelocity[i][j] = 0.0;
        }
    }
}

void ControlPanel::updateBuffer()
{
    for (int i = 40; i > 0; i--)
    {
        mBufferPosition[i][0] = mBufferPosition[i - 1][0];
        mBufferPosition[i][1] = mBufferPosition[i - 1][1];
        mBufferVelocity[i][0] = mBufferVelocity[i - 1][0];
        mBufferVelocity[i][1] = mBufferVelocity[i - 1][1];
    }
    mBufferPosition[0][0] = sharedMemory->motorPosition[0];
    mBufferPosition[0][1] = sharedMemory->motorPosition[1];
    mBufferVelocity[0][0] = sharedMemory->motorVelocity[0];
    mBufferVelocity[0][1] = sharedMemory->motorVelocity[1];
}

void ControlPanel::updateNetInputs()
{
    sharedMemory->NETInputs[0] = mBufferPosition[0][0];
    sharedMemory->NETInputs[1] = mBufferPosition[20][0];
    sharedMemory->NETInputs[2] = mBufferPosition[40][0];
    sharedMemory->NETInputs[3] = mBufferPosition[0][1];
    sharedMemory->NETInputs[4] = mBufferPosition[20][1];
    sharedMemory->NETInputs[5] = mBufferPosition[40][1];
    sharedMemory->NETInputs[6] = mBufferVelocity[0][0];
    sharedMemory->NETInputs[7] = mBufferVelocity[20][0];
    sharedMemory->NETInputs[8] = mBufferVelocity[40][0];
    sharedMemory->NETInputs[9] = mBufferVelocity[0][1];
    sharedMemory->NETInputs[10] = mBufferVelocity[20][1];
    sharedMemory->NETInputs[11] = mBufferVelocity[40][1];
    sharedMemory->NETInputs[12] = sharedMemory->motorTorque[0];
    sharedMemory->NETInputs[13] = sharedMemory->motorTorque[1];
}