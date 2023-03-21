//
// Created by hs on 22. 10. 27.
//

#include <canine-leg_simulation/SimulControlPanel.hpp>

extern pUI_COMMAND sharedCommand;
extern pSHM sharedMemory;

SimulControlPanel::SimulControlPanel(raisim::World* world, raisim::ArticulatedSystem* robot)
    : mWorld(world)
    , mRobot(robot)
    , mIteration(0)
{
    mTorque.setZero();
    clearBuffer();
}

void SimulControlPanel::ControllerFunction()
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
        PDcontrol.InitCubicTrajectory();
        sharedMemory->controlState = STATE_CUBIC_CONTROL;
        break;
    }
    case STATE_CUBIC_CONTROL:
    {
        PDcontrol.DoCubicControl();
        break;
    }
    case STATE_COS_READY:
    {
        PDcontrol.InitCosTrajectory();
        sharedMemory->controlState = STATE_COS_CONTROL;
        break;
    }
    case STATE_COS_CONTROL:
    {
        PDcontrol.DoCosControl();
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

void SimulControlPanel::integrateSimul()
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

void SimulControlPanel::updateStates()
{
//    double pastHipVerticalVelocity;
//    pastHipVerticalVelocity = sharedMemory->hipVerticalVelocity;
    sharedMemory->hipVerticalPosition = mRobot->getGeneralizedCoordinate()[0];
    sharedMemory->motorPosition[0] = mRobot->getGeneralizedCoordinate()[1];
    sharedMemory->motorPosition[1] = mRobot->getGeneralizedCoordinate()[2];
    sharedMemory->hipVerticalVelocity = mRobot->getGeneralizedVelocity()[0];
//    sharedMemory->hipVerticalAcceleration = (sharedMemory->hipVerticalVelocity - pastHipVerticalVelocity) / CONTROL_dT;
    sharedMemory->motorVelocity[0] = mRobot->getGeneralizedVelocity()[1];
    sharedMemory->motorVelocity[1] = mRobot->getGeneralizedVelocity()[2];

    sharedMemory->motorTorque[0] = sharedMemory->motorDesiredTorque[0];
    sharedMemory->motorTorque[1] = sharedMemory->motorDesiredTorque[1];

    if (&(mRobot->getContacts()[0]) != nullptr)
    {
        sharedMemory->measuredGRF = mRobot->getContacts()[0].getImpulse()[2] / CONTROL_dT;
    }
}

void SimulControlPanel::clearBuffer()
{
    for (int i = 0; i < BUFFER_SIZE ; i++)
    {
        for (int j = 0; j < 2 ; j++)
        {
            mBufferPosition[i][j] = 0.0;
            mBufferVelocity[i][j] = 0.0;
        }
    }
}

void SimulControlPanel::updateBuffer()
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

void SimulControlPanel::updateNetInputs()
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