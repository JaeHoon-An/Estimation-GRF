//
// Created by camel on 22. 9. 21.
//

#include <canine-leg_simulation/JointPDController.hpp>

extern pSHM sharedMemory;

JointPDController::JointPDController()
 : mIsFirstGenTraj(true)
{
    for (int motorIdx = 0; motorIdx < MOTOR_NUM; motorIdx++)
    {
        mTorqueLimit[motorIdx] = 13.0;
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

void JointPDController::DoCosControl()
{
    setCosTrajectory();
    solveIK();
    computeControlInput();
    SetControlInput();
}

void JointPDController::InitHomeTrajectory()
{
    double homeHip = 60;
    double homeKnee = -120;
    double timeDuration = 2.0;
    mIsFirstGenTraj = true;
    mCubicTrajectoryGen[0].updateTrajectory(sharedMemory->motorPosition[0], homeHip * D2R, sharedMemory->localTime, timeDuration);
    mCubicTrajectoryGen[1].updateTrajectory(sharedMemory->motorPosition[1], homeKnee * D2R, sharedMemory->localTime, timeDuration);
}

void JointPDController::InitCubicTrajectory()
{
    mIsFirstGenTraj = true;
    mCubicTrajectoryGenHipVertical.updateTrajectory(sharedMemory->desiredHipVerticalPosition, sharedMemory->cubicGoalHeight, sharedMemory->localTime, sharedMemory->cubicTimeDuration);
}

void JointPDController::InitCosTrajectory()
{
    mIsFirstGenTraj = true;
    mCosTrajectoryGenerator.updateTrajectory(sharedMemory->desiredHipVerticalPosition, sharedMemory->localTime, sharedMemory->cosAmplitude, sharedMemory->cosFrequency);
}

void JointPDController::solveIK()
{
    mPastDesiredPosition[0] = mDesiredPosition[0];
    mPastDesiredPosition[1] = mDesiredPosition[1];
    mDesiredPosition[0] = acos(sharedMemory->desiredHipVerticalPosition / 0.46);
    mDesiredPosition[1] = -2*mDesiredPosition[0];

    if (mIsFirstGenTraj)
    {
        mPastDesiredPosition[0] = mDesiredPosition[0];
        mPastDesiredPosition[1] = mDesiredPosition[1];
        mIsFirstGenTraj = false;
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
