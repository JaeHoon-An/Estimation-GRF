//
// Created by hs on 22. 10. 27.
//

#include <canine-leg_simulation/SimulMain.hpp>

pthread_t RTThreadController;
pthread_t NRTThreadCommand;

pUI_COMMAND sharedCommand;
pSHM sharedMemory;

raisim::World world;
raisim::RaisimServer server(&world);
raisim::ArticulatedSystem* robot = world.addArticulatedSystem(std::string(URDF_RSC_DIR) + "/canine_leg_left/canine_leg_left.urdf");

SimulCommand userCommand;
SimulVisualizer Visualizer(&world, robot, &server);
SimulControlPanel ControlPanel(&world, robot);

void* NRTCommandThread(void* arg)
{
    std::cout << "entered #nrt_command_thread" << std::endl;
    while (true)
    {
        userCommand.commandFunction();
        usleep(CMD_dT * 1e6);
    }
}

void* RTControllerThread(void* arg)
{
    std::cout << "entered #rt_controller_thread" << std::endl;
    struct timespec TIME_NEXT;
    struct timespec TIME_NOW;
    const long PERIOD_US = long(CONTROL_dT * 1e6); // 200Hz 짜리 쓰레드

    clock_gettime(CLOCK_REALTIME, &TIME_NEXT);
    std::cout << "bf #while" << std::endl;
    std::cout << "control freq : " << 1 / double(PERIOD_US) * 1e6 << std::endl;

    while (true)
    {
        clock_gettime(CLOCK_REALTIME, &TIME_NOW); //현재 시간 구함
        timespec_add_us(&TIME_NEXT, PERIOD_US);   //목표 시간 구함

        ControlPanel.ControllerFunction();

        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &TIME_NEXT, NULL); //목표시간까지 기다림 (현재시간이 이미 오바되어 있으면 바로 넘어갈 듯)
        if (timespec_cmp(&TIME_NOW, &TIME_NEXT) > 0)
        {
            std::cout << "RT Deadline Miss, Controller thread : " << timediff_us(&TIME_NEXT, &TIME_NOW) * 0.001
                << " ms" << std::endl;
        }

    }
}

void clearSharedMemory()
{
    sharedMemory->newCommand = false;
    sharedMemory->canStatus = false;
    sharedMemory->motorStatus = false;
    sharedMemory->controlState = STATE_CONTROL_STOP;
    sharedMemory->visualState = VISUAL_STOP;
    sharedMemory->canState = CAN_NO_ACT;
    sharedMemory->localTime = 0;
    sharedMemory->desiredHipVerticalPosition = 0;
    sharedMemory->desiredHipVerticalVelocity = 0;
    sharedMemory->hipVerticalPosition = 0;
    sharedMemory->hipVerticalVelocity = 0;
    sharedMemory->measuredGRF = 0;
    sharedMemory->estimatedGRF = 0;

    for (int index = 0; index < MOTOR_NUM; index++)
    {
        sharedMemory->motorErrorStatus[index] = 0;
        sharedMemory->motorTemp[index] = 0;
        sharedMemory->motorVoltage[index] = 0;
        sharedMemory->motorPosition[index] = 0;
        sharedMemory->motorDesiredPosition[index] = 0;
        sharedMemory->motorVelocity[index] = 0;
        sharedMemory->motorDesiredVelocity[index] = 0;
        sharedMemory->motorTorque[index] = 0;
        sharedMemory->motorDesiredTorque[index] = 0;
        sharedMemory->motorPrevDesiredPosition[index] = 0;
    }

    sharedMemory->Kp = DEFAULT_Kp;
    sharedMemory->Kd = DEFAULT_Kd;
    sharedMemory->LPFCutoffFrequency = DEFAULT_CUTOFF_FREQ;
    sharedMemory->cubicGoalHeight = DEFAULT_CUBIC_HEIGHT;
    sharedMemory->cosAmplitude = DEFAULT_COS_AMPLITUDE;
    sharedMemory->cosFrequency = DEFAULT_COS_FREQUENCY;
    sharedMemory->learningRate = DEFAULT_LEARNING_RATE;
}

void StartSimulation()
{
    sharedCommand = (pUI_COMMAND)malloc(sizeof(UI_COMMAND));
    sharedMemory = (pSHM)malloc(sizeof(SHM));
    clearSharedMemory();

    server.launchServer(8080);
    int thread_id_rt1 = generate_rt_thread(RTThreadController, RTControllerThread, "rt_thread1", 6, 99, NULL);
    int thread_id_nrt1 = generate_nrt_thread(NRTThreadCommand, NRTCommandThread, "nrt_thread1", 1, NULL);
}