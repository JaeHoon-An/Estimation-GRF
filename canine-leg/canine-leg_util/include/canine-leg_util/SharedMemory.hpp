//
// Created by jaehoon on 23. 3. 15.
//

#ifndef ESTIMATION_GRF_SHAREDMEMORY_HPP
#define ESTIMATION_GRF_SHAREDMEMORY_HPP

#include <string>

constexpr int MAX_COMMAND_DATA = 10;
constexpr int MAX_CUSTOM_DATA = 20;
constexpr int MOTOR_NUM = 2;
constexpr int NET_INPUT_BUFFER_SIZE = 41;
constexpr int GRF_BUFFER_SIZE = 3;

constexpr int GRF_NET_INPUT_SIZE = 14;
constexpr int GRF_NET_OUTPUT_SIZE = 1;
constexpr int SIM2REAL_NET_INPUT_SIZE = 10;
constexpr int SIM2REAL_NET_OUTPUT_SIZE = 1;

constexpr double DEFAULT_Kp = 70.0;
constexpr double DEFAULT_Kd = 2.5;
constexpr double DEFAULT_CUTOFF_FREQ = 50.0;
constexpr double DEFAULT_CUBIC_HEIGHT = 0.35;
constexpr double DEFAULT_CUBIC_TIME = 2.0;
constexpr double DEFAULT_COS_AMPLITUDE = 0.05;
constexpr double DEFAULT_COS_FREQUENCY = 0.5;
constexpr double DEFAULT_LEARNING_RATE = 0.001;

constexpr double CMD_dT = 0.001;
constexpr double CONTROL_dT = 0.001;
constexpr double TORCH_dT = 0.0005;
constexpr double CAN_dT = 0.0025;
constexpr double VISUAL_dT = 0.01;

constexpr double LINK1_LENGTH = 0.23;
constexpr double LINK2_LENGTH = 0.23;
constexpr int HIP_IDX = 0;
constexpr int KNEE_IDX = 1;
constexpr int MOTOR_HIP_ID = 0x145;
constexpr int MOTOR_KNEE_ID = 0x141;

constexpr double HIP_POS_OFFSET = 1.1882;
constexpr double KNEE_POS_OFFSET = -3.260096;

typedef struct _UI_COMMAND_
{
    int userCommand;
} UI_COMMAND, * pUI_COMMAND;

typedef struct _SHM_
{
    bool newCommand;
    bool canStatus;
    bool motorStatus;
    bool dataCollectStopFlag;

    int controlState;
    int visualState;
    int canState;
    int torchState;
    int motorErrorStatus[MOTOR_NUM];
    int motorTemp[MOTOR_NUM];
    int dataIdx;
    double localTime;
    double desiredHipVerticalPosition;
    double desiredHipVerticalVelocity;
    double hipVerticalPosition;
    double hipVerticalVelocity;
    double measuredGRF;
    double estimatedGRF;
    double sim2realGRF;
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
    double GRFNETInputs[GRF_NET_INPUT_SIZE];
    double Sim2RealNETInputs[SIM2REAL_NET_INPUT_SIZE];

    double motionTableOffset[27];
    double motionTableAmplitude[27];
    double motionTableFrequency[27];
    double dataForTransferLearning[2000000][11];
    double dataForSupervisedLearning[2000000][15];
    double dataForResults[2000000][3];
    std::string* modelName;
} SHM, * pSHM;

enum CONTROL_STATE
{
    STATE_CONTROL_STOP,
    STATE_HOME_READY,
    STATE_HOME_CONTROL,
    STATE_CUBIC_READY,
    STATE_CUBIC_CONTROL,
    STATE_COS_READY,
    STATE_COS_CONTROL,
    STATE_DATA_CUBIC_READY,
    STATE_DATA_CUBIC_CONTROL,
    STATE_DATA_COS_READY,
    STATE_DATA_COS_CONTROL
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
    SIM_SAVE_DATA
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
    MOTOR_ON,
    MOTOR_OFF,
    HOME,
    SETPARAMS,
    CUBIC_CONTROL,
    COS_CONTROL,
    LOAD_MODEL,
    ONLINE_LEARNING,
    SAVE_DATA,
    CUSTOM1,
    CUSTOM2,
    CUSTOM3,
    CUSTOM4
};

enum CAN_STATE
{
    CAN_NO_ACT,
    CAN_INIT,
    CAN_MOTOR_ON,
    CAN_MOTOR_OFF,
    CAN_SET_TORQUE,
    CAN_SET_ZERO_TORQUE,
    CAN_READ_ERROR
};

enum TORCH_STATE
{
    TORCH_NO_ACT,
    TORCH_LOAD_MODEL,
    TORCH_ESTIMATION,
    TORCH_ONLINE_LEARNING
};
#endif //ESTIMATION_GRF_SHAREDMEMORY_HPP

