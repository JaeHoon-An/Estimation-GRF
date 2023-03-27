//
// Created by camel on 22. 9. 21.
//

#include <canine-leg_util/JointPDController.hpp>

extern pSHM sharedMemory;

JointPDController::JointPDController()
 : mbFirstGenTraj(true)
 , mbCollectFlag(false)
 , mIteration(0)
 , mPeriod(0)
{
    mGRFBuffer.setZero();
    for (int motorIdx = 0; motorIdx < MOTOR_NUM; motorIdx++)
    {
        mTorqueLimit[motorIdx] = 20.0;
    }
}

void JointPDController::DoHomeControl()
{
    setHomeTrajectory();
    computeControlInput();
    SetControlInput();
}

void JointPDController::DoCubicControl()
{
    setCubicTrajectory();
    solveIK();
    computeControlInput();
    SetControlInput();
}

void JointPDController::InitCosTrajectory()
{
    mbFirstGenTraj = true;
    mbCollectFlag = false;
    mIteration = 0;
    mPeriod = int(1 / sharedMemory->cosFrequency / CONTROL_dT);
    mCosTrajectoryGenerator.updateTrajectory(sharedMemory->desiredHipVerticalPosition, sharedMemory->localTime, sharedMemory->cosAmplitude, sharedMemory->cosFrequency);
}

void JointPDController::updateBuffer()
{
    for (int i = 9; i > 0; i--)
    {
        mGRFBuffer(i, 0) = mGRFBuffer(i - 1, 0);
    }
    mGRFBuffer(0, 0) = sharedMemory->estimatedGRF;
}

void JointPDController::collectData()
{
    if (sharedMemory->measuredGRF == 0)
    {
        std::cout << "[DATA COLLECTOR] Zero GRF is occurred." << std::endl;
        sharedMemory->dataCollectStopFlag = true;
    }
    else if (sharedMemory->measuredGRF > 100)
    {
        std::cout << "[DATA COLLECTOR] Raisim error is occurred." << std::endl;
        sharedMemory->dataCollectStopFlag = true;
    }
    else if(sharedMemory->hipVerticalPosition < 0.03)
    {
        std::cout << "[DATA COLLECTOR] Base contact is occurred." << std::endl;
        sharedMemory->dataCollectStopFlag = true;
    }
    else if (isnan(sharedMemory->motorPosition[0] + sharedMemory->motorPosition[1] + sharedMemory->motorVelocity[0] + sharedMemory->motorVelocity[1] + mTorque[1] + mTorque[2]))
    {
        std::cout << "[DATA COLLECTOR] nan is occurred." << std::endl;
        sharedMemory->dataCollectStopFlag = true;
    }
    else if(mbCollectFlag == true)
    {
        sharedMemory->dataForTransferLearning[sharedMemory->dataIdx][0] = mGRFBuffer(0, 0);
        sharedMemory->dataForTransferLearning[sharedMemory->dataIdx][1] = mGRFBuffer(1, 0);
        sharedMemory->dataForTransferLearning[sharedMemory->dataIdx][2] = mGRFBuffer(2, 0);
        sharedMemory->dataForTransferLearning[sharedMemory->dataIdx][3] = mGRFBuffer(3, 0);
        sharedMemory->dataForTransferLearning[sharedMemory->dataIdx][4] = mGRFBuffer(4, 0);
        sharedMemory->dataForTransferLearning[sharedMemory->dataIdx][5] = mGRFBuffer(5, 0);
        sharedMemory->dataForTransferLearning[sharedMemory->dataIdx][6] = mGRFBuffer(6, 0);
        sharedMemory->dataForTransferLearning[sharedMemory->dataIdx][7] = mGRFBuffer(7, 0);
        sharedMemory->dataForTransferLearning[sharedMemory->dataIdx][8] = mGRFBuffer(8, 0);
        sharedMemory->dataForTransferLearning[sharedMemory->dataIdx][9] = mGRFBuffer(9, 0);
        sharedMemory->dataForTransferLearning[sharedMemory->dataIdx][10] = sharedMemory->measuredGRF;
        sharedMemory->dataIdx += 1;
    }
}

void JointPDController::DoCosControl()
{
    mIteration++;
    setCosTrajectory();
    solveIK();
    computeControlInput();
    SetControlInput();
    updateBuffer();
    collectData();
    if(mIteration == mPeriod)
    {
        mbCollectFlag = true;
    }
    if ((mIteration == 2*mPeriod) || sharedMemory->dataCollectStopFlag)
    {
        std::cout << "[SYSTEM] iteration : " << mIteration << std::endl;
        std::cout << "[SYSTEM] data index : " << sharedMemory->dataIdx << std::endl<< std::endl;
        sharedMemory->dataCollectStopFlag = true;
    }
}

void JointPDController::InitHomeTrajectory()
{
    double homeHip = 60;
    double homeKnee = -120;
    double timeDuration = 2.0;
    mbFirstGenTraj = true;
    mCubicTrajectoryGen[0].updateTrajectory(sharedMemory->motorPosition[0], homeHip * D2R, sharedMemory->localTime, timeDuration);
    mCubicTrajectoryGen[1].updateTrajectory(sharedMemory->motorPosition[1], homeKnee * D2R, sharedMemory->localTime, timeDuration);
}

void JointPDController::InitCubicTrajectory()
{
    mbFirstGenTraj = true;
    mCubicTrajectoryGenHipVertical.updateTrajectory(sharedMemory->desiredHipVerticalPosition, sharedMemory->cubicGoalHeight, sharedMemory->localTime, 2.0);
}

void JointPDController::solveIK()
{
    mPastDesiredPosition[0] = mDesiredPosition[0];
    mPastDesiredPosition[1] = mDesiredPosition[1];
    mDesiredPosition[0] = acos(sharedMemory->desiredHipVerticalPosition / 0.46);
    mDesiredPosition[1] = -2*mDesiredPosition[0];

    if (mbFirstGenTraj)
    {
        mPastDesiredPosition[0] = mDesiredPosition[0];
        mPastDesiredPosition[1] = mDesiredPosition[1];
        mbFirstGenTraj = false;
    }

    mDesiredVelocity[0] = (mDesiredPosition[0] - mPastDesiredPosition[0]) / CONTROL_dT;
    mDesiredVelocity[1] = (mDesiredPosition[1] - mPastDesiredPosition[1]) / CONTROL_dT;

    for(int i = 0 ; i <MOTOR_NUM ; i++)
    {
        sharedMemory->motorDesiredPosition[i] = mDesiredPosition[i];
        sharedMemory->motorDesiredVelocity[i] = mDesiredVelocity[i];
    }
}

void JointPDController::setHomeTrajectory()
{
    for (int index = 0; index < MOTOR_NUM; index++)
    {
        mDesiredPosition[index] = mCubicTrajectoryGen[index].getPositionTrajectory(sharedMemory->localTime);
        mDesiredVelocity[index] = mCubicTrajectoryGen[index].getVelocityTrajectory(sharedMemory->localTime);
    }
    double pastDesiredHipVerticalPosition = sharedMemory->desiredHipVerticalPosition;
    sharedMemory->desiredHipVerticalPosition = 0.23 * cos(mDesiredPosition[0])+0.23 * cos(mDesiredPosition[0]+mDesiredPosition[1]);
    sharedMemory->desiredHipVerticalVelocity = (sharedMemory->desiredHipVerticalPosition - pastDesiredHipVerticalPosition) / CONTROL_dT;
    for(int i = 0 ; i <MOTOR_NUM ; i++)
    {
        sharedMemory->motorDesiredPosition[i] = mDesiredPosition[i];
        sharedMemory->motorDesiredVelocity[i] = mDesiredVelocity[i];
    }
}

void JointPDController::setCubicTrajectory()
{
    sharedMemory->desiredHipVerticalPosition = mCubicTrajectoryGenHipVertical.getPositionTrajectory(sharedMemory->localTime);
    sharedMemory->desiredHipVerticalVelocity = mCubicTrajectoryGenHipVertical.getVelocityTrajectory(sharedMemory->localTime);
}

void JointPDController::setCosTrajectory()
{
    sharedMemory->desiredHipVerticalPosition = mCosTrajectoryGenerator.getPositionTrajectory(sharedMemory->localTime);
    sharedMemory->desiredHipVerticalVelocity = mCosTrajectoryGenerator.getVelocityTrajectory(sharedMemory->localTime);
}

void JointPDController::computeControlInput()
{
    for (int index = 0; index < MOTOR_NUM; index++)
    {
        mTorque[index] = sharedMemory->Kp * (mDesiredPosition[index] - sharedMemory->motorPosition[index])
            + sharedMemory->Kd * (mDesiredVelocity[index] - sharedMemory->motorVelocity[index]);
    }
}

void JointPDController::SetControlInput()
{
    for (int index = 0; index < MOTOR_NUM; index++)
    {
        if (mTorque[index] > mTorqueLimit[index])
        {
            mTorque[index] = mTorqueLimit[index];
        }
        else if (mTorque[index] < -mTorqueLimit[index])
        {
            mTorque[index] = -mTorqueLimit[index];
        }
        sharedMemory->motorDesiredTorque[index] = mTorque[index];
    }
}
