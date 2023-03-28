//
// Created by hs on 22. 10. 28.
//

#include <canine-leg_real/Command.hpp>

extern pUI_COMMAND sharedCommand;
extern pSHM sharedMemory;

Command::Command()
{

}

void Command::commandFunction()
{
    if (sharedMemory->newCommand)
    {
        sharedMemory->newCommand = false;
        int incomingCommand = sharedCommand->userCommand;

        switch (incomingCommand)
        {
        case NO_ACT:
        {
            break;
        }
        case CAN_ON:
        {
            sharedMemory->canState = CAN_INIT;
            break;
        }
        case MOTOR_ON:
        {
            sharedMemory->canState = CAN_MOTOR_ON;
            break;
        }
        case MOTOR_OFF:
        {
            sharedMemory->canState = CAN_MOTOR_OFF;
            break;
        }
        case HOME:
        {
            sharedMemory->controlState = STATE_HOME_READY;
            sharedMemory->canState = CAN_SET_TORQUE;
            break;
        }
        case SETPARAMS:
        {
            break;
        }
        case CUBIC_CONTROL:
        {
            sharedMemory->controlState = STATE_CUBIC_READY;
            sharedMemory->canState = CAN_SET_TORQUE;
            break;
        }
        case COS_CONTROL:
        {
            sharedMemory->controlState = STATE_COS_READY;
            sharedMemory->canState = CAN_SET_TORQUE;
            break;
        }
        case LOAD_MODEL:
        {
            sharedMemory->torchState = TORCH_LOAD_MODEL;
            break;
        }
        case ONLINE_LEARNING:
        {
            sharedMemory->torchState = TORCH_ONLINE_LEARNING;
            break;
        }
        case SAVE_DATA:
        {
            writeToCSVfile();
            break;
        }
        case CUSTOM1:
        {
            sharedMemory->controlState = STATE_DATA_CUBIC_READY;
            break;
        }
        case CUSTOM2:
        {
            break;
        }
        case CUSTOM3:
        {
            break;
        }
        case CUSTOM4:
        {
            sharedMemory->canState = CAN_SET_ZERO_TORQUE;
            break;
        }
        default:
            break;
        }
    }
}

void Command::writeToCSVfile()
{
    std::string name2;
    name2.append(DATASET_DIR);
    name2.append("GRFDatasetsRealWorld.csv");
    std::ofstream file2(name2.c_str());
    for (int i = 0; i < sharedMemory->dataIdx; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            std::string str = std::to_string(sharedMemory->dataForTransferLearning[i][j]);
            if (j + 1 == 11)
            {
                file2 << str;
            }
            else
            {
                file2 << str << ',';
            }
        }
        file2 << '\n';
    }
    std::cout << "Data is saved." << std::endl;
}