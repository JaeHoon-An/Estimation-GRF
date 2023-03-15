//
// Created by jaehoon on 23. 3. 15.
//

#ifndef ESTIMATION_GRF_SHAREDMEMORY_HPP
#define ESTIMATION_GRF_SHAREDMEMORY_HPP

constexpr int MAX_COMMAND_DATA = 10;
constexpr int MAX_CUSTOM_DATA = 20;
constexpr int MOTOR_NUM = 2;

constexpr int HIP_IDX = 0;
constexpr int KNEE_IDX = 1;
constexpr int MOTOR_HIP_ID = 0x141;
constexpr int MOTOR_KNEE_ID = 0x142;

constexpr double CMD_dT = 0.001;
constexpr double CONTROL_dT = 0.001;
constexpr double CAN_dT = 0.0025;
constexpr double VISUAL_dT = 0.01;
constexpr double PI = 3.141592;
constexpr double R2D = 57.2957802;
constexpr double D2R = 0.0174533;

typedef struct _UI_COMMAND_
{
    int userCommand;
} UI_COMMAND, * pUI_COMMAND;

typedef struct _SHM_
{
    bool newCommand;
    bool canStatus;
    bool motorStatus;

    int ControlState;
    int visualState;
    int canState;
    int motorErrorStatus[MOTOR_NUM];
    int motorTemp[MOTOR_NUM];

    double localTime;
    double motorPosition[MOTOR_NUM];
    double motorVelocity[MOTOR_NUM];
    double motorTorque[MOTOR_NUM];
    double motorDesiredPosition[MOTOR_NUM];
    double motorPrevDesiredPosition[MOTOR_NUM];
    double motorDesiredVelocity[MOTOR_NUM];
    double motorDesiredTorque[MOTOR_NUM];
    double motorVoltage[MOTOR_NUM];
} SHM, * pSHM;

#endif //ESTIMATION_GRF_SHAREDMEMORY_HPP
