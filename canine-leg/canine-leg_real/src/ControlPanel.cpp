//
// Created by hs on 22. 10. 27.
//

#include <canine-leg_real/ControlPanel.hpp>

extern pUI_COMMAND sharedCommand;
extern pSHM sharedMemory;

ControlPanel::ControlPanel()
    : mIteration(0)
    , mMotionTableIdx(0)
    , mReferenceTime(0.0)
{
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
    case STATE_DATA_CUBIC_READY:
    {
        sharedMemory->cubicGoalHeight = sharedMemory->motionTableOffset[mMotionTableIdx];
        sharedMemory->cosAmplitude = sharedMemory->motionTableAmplitude[mMotionTableIdx];
        sharedMemory->cosFrequency = sharedMemory->motionTableFrequency[mMotionTableIdx];
        mMotionTableIdx++;
        std::cout << "[SYSTEM] offset, amplitude, frequency : " << sharedMemory->cubicGoalHeight << ", " << sharedMemory->cosAmplitude << ", " << sharedMemory->cosFrequency << std::endl;\
        PDcontrol.InitCubicTrajectory();
        mReferenceTime = sharedMemory->localTime;
        sharedMemory->dataCollectStopFlag = false;
        sharedMemory->controlState = STATE_DATA_CUBIC_CONTROL;
        break;
    }
    case STATE_DATA_CUBIC_CONTROL:
    {
        PDcontrol.DoCubicControl();
        if (sharedMemory->localTime >= mReferenceTime + 2.0)
        {
            sharedMemory->controlState = STATE_DATA_COS_READY;
        }
        break;
    }
    case STATE_DATA_COS_READY:
    {
        PDcontrol.InitCosTrajectory();
        mReferenceTime = sharedMemory->localTime;
        sharedMemory->controlState = STATE_DATA_COS_CONTROL;
        break;
    }
    case STATE_DATA_COS_CONTROL:
    {
        PDcontrol.DoCosControl();
        if (sharedMemory->dataCollectStopFlag)
        {
            sharedMemory->controlState = STATE_DATA_CUBIC_READY;
            if (mMotionTableIdx == 27)
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
    updateBuffer();
    updateNetInputs();
}

void ControlPanel::clearBuffer()
{
    for (int i = 0; i < NET_INPUT_BUFFER_SIZE; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            mBufferPosition[i][j] = 0.0;
            mBufferVelocity[i][j] = 0.0;
        }
    }
    mGRFBuffer.setZero();
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

    for (int i = 9; i > 0; i--)
    {
        mGRFBuffer(i, 0) = mGRFBuffer(i - 1, 0);
    }
    mGRFBuffer(0, 0) = sharedMemory->estimatedGRF;
}

void ControlPanel::updateNetInputs()
{
    sharedMemory->GRFNETInputs[0] = mBufferPosition[0][0];
    sharedMemory->GRFNETInputs[1] = mBufferPosition[20][0];
    sharedMemory->GRFNETInputs[2] = mBufferPosition[40][0];
    sharedMemory->GRFNETInputs[3] = mBufferPosition[0][1];
    sharedMemory->GRFNETInputs[4] = mBufferPosition[20][1];
    sharedMemory->GRFNETInputs[5] = mBufferPosition[40][1];
    sharedMemory->GRFNETInputs[6] = mBufferVelocity[0][0];
    sharedMemory->GRFNETInputs[7] = mBufferVelocity[20][0];
    sharedMemory->GRFNETInputs[8] = mBufferVelocity[40][0];
    sharedMemory->GRFNETInputs[9] = mBufferVelocity[0][1];
    sharedMemory->GRFNETInputs[10] = mBufferVelocity[20][1];
    sharedMemory->GRFNETInputs[11] = mBufferVelocity[40][1];
    sharedMemory->GRFNETInputs[12] = sharedMemory->motorDesiredTorque[0];
    sharedMemory->GRFNETInputs[13] = sharedMemory->motorDesiredTorque[1];

    sharedMemory->Sim2RealNETInputs[0] = mGRFBuffer(0, 0);
    sharedMemory->Sim2RealNETInputs[1] = mGRFBuffer(1, 0);
    sharedMemory->Sim2RealNETInputs[2] = mGRFBuffer(2, 0);
    sharedMemory->Sim2RealNETInputs[3] = mGRFBuffer(3, 0);
    sharedMemory->Sim2RealNETInputs[4] = mGRFBuffer(4, 0);
    sharedMemory->Sim2RealNETInputs[5] = mGRFBuffer(5, 0);
    sharedMemory->Sim2RealNETInputs[6] = mGRFBuffer(6, 0);
    sharedMemory->Sim2RealNETInputs[7] = mGRFBuffer(7, 0);
    sharedMemory->Sim2RealNETInputs[8] = mGRFBuffer(8, 0);
    sharedMemory->Sim2RealNETInputs[9] = mGRFBuffer(9, 0);
}