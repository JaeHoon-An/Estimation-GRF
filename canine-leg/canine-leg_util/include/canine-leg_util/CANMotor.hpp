//
// Created by jaehoon on 23. 3. 27.
//

#ifndef ESTIMATION_GRF_CANMOTOR_HPP
#define ESTIMATION_GRF_CANMOTOR_HPP

#include <iostream>
#include <unistd.h>
#include <cmath>
#include <net/if.h>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include "SharedMemory.hpp"
#include "CustomMath.hpp"

class CanMotor
{
public:
    CanMotor(std::string canName);
    void CanFunction();

private:
    void canInit();
    void canSend(int motorIndex, const u_int8_t* data);
    void canRead();
    void readEncoder();
    void readMotorErrorStatus();
    void turnOnMotor();
    void turnOffMotor();
    void setTorque();

private:
    double enc2rad;
    double torque2int[MOTOR_NUM];
    std::string mCanName;
    struct can_frame mFrame;
    int mMotorId[MOTOR_NUM];
    int mEncoder[MOTOR_NUM];
    int mEncoderMultiturnNum[MOTOR_NUM];
    int mEncoderTemp[MOTOR_NUM];
    int mEncoderPast[MOTOR_NUM];
    int mEncoderRaw[MOTOR_NUM];
    int mEncoderOffset[MOTOR_NUM];
    int mSock;
    int mGearRatio;
    int mMotorTemperature[MOTOR_NUM];
    int mMotorErrorCode[MOTOR_NUM];
    double mAxis[MOTOR_NUM];
    double mAngularPositionOffset[MOTOR_NUM];
    double mAngularPosition[MOTOR_NUM];
    double mAngularVelocity[MOTOR_NUM];
    double mCurrentTorque[MOTOR_NUM];
    double mMotorVoltage[MOTOR_NUM];

};


#endif //ESTIMATION_GRF_CANMOTOR_HPP
