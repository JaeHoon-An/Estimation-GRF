//
// Created by jaehoon on 23. 3. 15.
//

#ifndef ESTIMATION_GRF_SHAREDMEMORY_HPP
#define ESTIMATION_GRF_SHAREDMEMORY_HPP

#include <string>

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

constexpr double DEFAULT_Kp = 70.0;
constexpr double DEFAULT_Kd = 2.5;
constexpr double DEFAULT_CUTOFF_FREQ = 50.0;
constexpr double DEFAULT_CUBIC_HEIGHT = 0.35;
constexpr double DEFAULT_CUBIC_TIME = 2.0;
constexpr double DEFAULT_COS_AMPLITUDE = 0.05;
constexpr double DEFAULT_COS_FREQUENCY = 0.5;
constexpr double DEFAULT_LEARNING_RATE = 0.001;

typedef struct _UI_COMMAND_
{
    int userCommand;
} UI_COMMAND, * pUI_COMMAND;

typedef struct _SHM_
{
    bool newCommand;
    bool canStatus;
    bool motorStatus;

    int controlState;
    int visualState;
    int canState;
    int motorErrorStatus[MOTOR_NUM];
    int motorTemp[MOTOR_NUM];

    double localTime;
    double desiredHipVerticalPosition;
    double desiredHipVerticalVelocity;
    double hipVerticalPosition;
    double hipVerticalVelocity;
    double measuredGRF;
    double estimatedGRF;
    double motorPosition[MOTOR_NUM];
    double motorVelocity[MOTOR_NUM];
    double motorTorque[MOTOR_NUM];
    double motorDesiredPosition[MOTOR_NUM];
    double motorPrevDesiredPosition[MOTOR_NUM];
    double motorDesiredVelocity[MOTOR_NUM];
    double motorDesiredTorque[MOTOR_NUM];
    double motorVoltage[MOTOR_NUM];

    double Kp;
    double Kd;
    double LPFCutoffFrequency;
    double cubicGoalHeight;
    double cubicTimeDuration;
    double cosAmplitude;
    double cosFrequency;
    double learningRate;
    std::string modelName;
    std::string dataSavePath;
} SHM, * pSHM;

enum CONTROL_STATE
{
    STATE_CONTROL_STOP,
    STATE_HOME_READY,
    STATE_HOME_CONTROL,
    STATE_CUBIC_READY,
    STATE_CUBIC_CONTROL,
    STATE_COS_READY,
    STATE_COS_CONTROL
};

enum SIMULATION_COMMAND
{
    SIM_NO_ACT,
    SIM_MOTOR_ON,
    SIM_MOTOR_OFF,
    SIM_HOME,
    SIM_SETPARAMS,
    SIM_CUBIC_CONTROL,
    SIM_COS_CONTROL,
    SIM_LOAD_MODEL,
    SIM_ONLINE_LEARNING,
    SIM_SAVE_RESULT
};

enum VISUAL_STATE
{
    VISUAL_STOP,
    VISUAL_OPEN_RAISIM,
    VISUAL_UPDATE_RAISIM
};

enum COMMAND
{
    NO_ACT,
    CAN_ON,
    VISUAL_ON,
    MOTOR_ON,
    MOTOR_OFF,
    RESET,
    HOME,
    PD_CMD,
};

enum CAN_STATE
{
    CAN_NO_ACT
};
#endif //ESTIMATION_GRF_SHAREDMEMORY_HPP

