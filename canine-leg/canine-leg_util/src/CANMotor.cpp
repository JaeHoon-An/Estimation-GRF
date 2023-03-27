//
// Created by jaehoon on 23. 3. 27.
//
#include <canine-leg_util/CANMotor.hpp>
#include <camel-tools/ThreadGenerator.hpp>

extern pSHM sharedMemory;

CanMotor::CanMotor(std::string canName)
    : mCanName(canName)
{
    enc2rad = 2.0 * 3.141592 / 65535;
    mSock = 0;
//    mGearRatio = 6;
//    torque2int[HIP_IDX] = 71.0227;
//    torque2int[KNEE_IDX] = 71.0227;

    mGearRatio = 9;
    torque2int[HIP_IDX] = 24.0385;
    torque2int[KNEE_IDX] = 24.0385;

    mMotorId[HIP_IDX] = MOTOR_HIP_ID;
    mMotorId[KNEE_IDX] = MOTOR_KNEE_ID;

    mAxis[HIP_IDX] = 1.0;
    mAxis[KNEE_IDX] = 1.0;

    mAngularPositionOffset[HIP_IDX] = HIP_POS_OFFSET;
    mAngularPositionOffset[KNEE_IDX] = KNEE_POS_OFFSET;

    for (int index = 0; index < MOTOR_NUM; index++)
    {
        mEncoder[index] = 0;
        mEncoderMultiturnNum[index] = 0;
        mEncoderTemp[index] = 35000;
        mEncoderPast[index] = 35000;
        mEncoderRaw[index] = 0;
        mEncoderOffset[index] = 0;
        mMotorTemperature[index] = 0;
        mMotorErrorCode[index] = 0;
        mAngularPosition[index] = 0;
        mAngularVelocity[index] = 0;
        mCurrentTorque[index] = 0;
        mMotorVoltage[index] = 0;
    }
}

void CanMotor::CanFunction()
{
    switch (sharedMemory->canState)
    {
    case CAN_NO_ACT:
    {
        break;
    }
    case CAN_INIT:
    {
        canInit();
        sharedMemory->canState = CAN_READ_ERROR;
        break;
    }
    case CAN_MOTOR_ON:
    {
        turnOnMotor();
        sharedMemory->canState = CAN_NO_ACT;
        readEncoder();
        break;
    }
    case CAN_MOTOR_OFF:
    {
        turnOffMotor();
        sharedMemory->canState = CAN_NO_ACT;
        break;
    }
    case CAN_SET_TORQUE:
    {
        setTorque();
        break;
    }
    case CAN_READ_ERROR:
    {
        readMotorErrorStatus();
        break;
    }
    default:
        break;
    }
}

void CanMotor::canInit()
{
    std::string command3 =
        "sudo ip link set " + mCanName + " up type can bitrate 1000000";
    const char* c3 = command3.c_str();
    system(c3);
    mSock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (mSock == -1)
    {
        perror("Fail to create can socket for ");
        std::cout << mCanName << std::endl;
        return;
    }
    std::cout << "Success to create can socket for " << mCanName << std::endl;

    struct ifreq ifr;
    const char* canName = mCanName.c_str();
    strcpy(ifr.ifr_name, canName);
    int ret = ioctl(mSock, SIOCGIFINDEX, &ifr);
    if (ret == -1)
    {
        perror("Fail to get can interface index -");
        return;
    }
    std::cout << "Success to get can interface index: " << ifr.ifr_ifindex << std::endl;

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(mSock, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("Fail to bind can socket -");
        return;
    }
    std::cout << "Success to bind can socket" << std::endl;
    sharedMemory->canStatus = true;
}

void CanMotor::canSend(int motorIndex, const u_int8_t* data)
{
    u_int32_t tempid = mMotorId[motorIndex] & 0x1fffffff;
    mFrame.can_id = tempid;
    memcpy(mFrame.data, data, sizeof(data));
    mFrame.can_dlc = sizeof(data);
    int tx_bytes = write(mSock, &mFrame, sizeof(mFrame));
    if (tx_bytes == -1)
    {
        perror("Fail to transmit can");
        return;
    }
}

void CanMotor::canRead()
{
    read(mSock, &mFrame, sizeof(mFrame));
}

void CanMotor::readEncoder()
{
    for (int motorIndex = 0; motorIndex < MOTOR_NUM; motorIndex++)
    {
        u_int8_t data[8] = { 0X90, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 };
        canSend(motorIndex, data);
        canRead();

        mEncoderPast[motorIndex] = mEncoderTemp[motorIndex];
        mEncoderTemp[motorIndex] = mFrame.data[2] + mFrame.data[3] * 256;
        mEncoderRaw[motorIndex] = mFrame.data[4] + mFrame.data[5] * 256;
        mEncoderOffset[motorIndex] = mFrame.data[6] + mFrame.data[7] * 256;
        if ((mEncoderTemp[motorIndex] < 10000) && (mEncoderPast[motorIndex] > 50000))
        {
            mEncoderMultiturnNum[motorIndex] += 1;
        }
        else if ((mEncoderTemp[motorIndex] > 50000) && (mEncoderPast[motorIndex] < 10000))
        {
            mEncoderMultiturnNum[motorIndex] -= 1;
        }
        mEncoder[motorIndex] = mEncoderTemp[motorIndex] + 65535 * mEncoderMultiturnNum[motorIndex];
        mAngularPosition[motorIndex] = mEncoder[motorIndex] * enc2rad / mGearRatio;
    }

    for (int motorIndex = 0; motorIndex < MOTOR_NUM; motorIndex++)
    {
        sharedMemory->motorPosition[motorIndex] = mAxis[motorIndex] * (mAngularPosition[motorIndex] + mAngularPositionOffset[motorIndex]);
    }
    sharedMemory->hipVerticalPosition = LINK1_LENGTH * cos(sharedMemory->motorPosition[HIP_IDX]) + LINK1_LENGTH * cos(sharedMemory->motorPosition[HIP_IDX] + sharedMemory->motorPosition[KNEE_IDX]);
}

void CanMotor::readMotorErrorStatus()
{
    for (int motorIndex = 0; motorIndex < MOTOR_NUM; motorIndex++)
    {
        u_int8_t data[8] = { 0X9a, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 };
        canSend(motorIndex, data);
        canRead();
        mMotorTemperature[motorIndex] = mFrame.data[1];
        mMotorVoltage[motorIndex] = (mFrame.data[3] + mFrame.data[4] * 256) * 0.1;
        mMotorErrorCode[motorIndex] = mFrame.data[7];
        sharedMemory->motorTemp[motorIndex] = mMotorTemperature[motorIndex];
        sharedMemory->motorVoltage[motorIndex] = mMotorVoltage[motorIndex];
        sharedMemory->motorErrorStatus[motorIndex] = mMotorErrorCode[motorIndex];
    }
}

void CanMotor::turnOffMotor()
{
    for (int motorIndex = 0; motorIndex < MOTOR_NUM; motorIndex++)
    {
        u_int8_t data[8] = { 0x80, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 };
        canSend(motorIndex, data);
        canRead();
    }
    sharedMemory->motorStatus = false;
}

void CanMotor::turnOnMotor()
{
    for (int motorIndex = 0; motorIndex < MOTOR_NUM; motorIndex++)
    {
        u_int8_t data[8] = { 0x88, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 };
        canSend(motorIndex, data);
        canRead();
    }
    sleep(5);
    sharedMemory->motorStatus = true;
}

void CanMotor::setTorque()
{
    double desiredTorque[MOTOR_NUM];
    for (int motorIndex = 0; motorIndex < MOTOR_NUM; motorIndex++)
    {
        desiredTorque[motorIndex] = sharedMemory->motorDesiredTorque[motorIndex];
    }

    for (int motorIndex = 0; motorIndex < MOTOR_NUM; motorIndex++)
    {
        u_int16_t desiredCurrent = round(mAxis[motorIndex] * torque2int[motorIndex] * desiredTorque[motorIndex]);
        u_int8_t currentLowerData;
        u_int8_t currentUpperData;

        currentLowerData = desiredCurrent % 256;
        desiredCurrent = desiredCurrent / 256;
        currentUpperData = desiredCurrent % 256;

//        currentLowerData = 0.0;
//        currentUpperData = 0.0;

        u_int8_t data[8] = { 0Xa1, 0X00, 0X00, 0X00, currentLowerData, currentUpperData, 0X00, 0X00 };

        canSend(motorIndex, data);
        canRead();


        int16_t currentTorque;
        int16_t angularVelocity;

        mMotorTemperature[motorIndex] = mFrame.data[1];
        currentTorque = mFrame.data[2] + mFrame.data[3] * 256;
        mCurrentTorque[motorIndex] = currentTorque / torque2int[motorIndex];
        angularVelocity = mFrame.data[4] + mFrame.data[5] * 256;
        mAngularVelocity[motorIndex] = angularVelocity * D2R / mGearRatio;

        mEncoderPast[motorIndex] = mEncoderTemp[motorIndex];
        mEncoderTemp[motorIndex] = mFrame.data[6] + mFrame.data[7] * 256;
        if ((mEncoderTemp[motorIndex] < 10000) && (mEncoderPast[motorIndex] > 50000))
        {
            mEncoderMultiturnNum[motorIndex] += 1;
        }
        else if ((mEncoderTemp[motorIndex] > 50000) && (mEncoderPast[motorIndex] < 10000))
        {
            mEncoderMultiturnNum[motorIndex] -= 1;
        }
        mEncoder[motorIndex] = mEncoderTemp[motorIndex] + 65535 * mEncoderMultiturnNum[motorIndex];
        mAngularPosition[motorIndex] = mEncoder[motorIndex] * enc2rad / mGearRatio;
    }

    for (int motorIndex = 0; motorIndex < MOTOR_NUM; motorIndex++)
    {
        sharedMemory->motorTemp[motorIndex] = mMotorTemperature[motorIndex];
        sharedMemory->motorTorque[motorIndex] = mAxis[motorIndex] * mCurrentTorque[motorIndex];
        sharedMemory->motorVelocity[motorIndex] = mAxis[motorIndex] * mAngularVelocity[motorIndex];
        sharedMemory->motorPosition[motorIndex] = mAxis[motorIndex] * (mAngularPosition[motorIndex] + mAngularPositionOffset[motorIndex]);
    }
    double pastHipVerticalPosition = sharedMemory->hipVerticalPosition;
    sharedMemory->hipVerticalPosition = LINK1_LENGTH * cos(sharedMemory->motorPosition[HIP_IDX]) + LINK1_LENGTH * cos(sharedMemory->motorPosition[HIP_IDX] + sharedMemory->motorPosition[KNEE_IDX]);
    sharedMemory->hipVerticalVelocity = (sharedMemory->hipVerticalPosition - pastHipVerticalPosition) / CONTROL_dT;
}
