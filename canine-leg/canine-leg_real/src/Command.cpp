//
// Created by hs on 22. 10. 28.
//

#include <canine-leg_real/Command.hpp>

extern pUI_COMMAND sharedCommand;
extern pSHM sharedMemory;

Command::Command()
: mbOnlineLearning(true)
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
            break;
        }
        case MOTOR_ON:
        {
            break;
        }
        case MOTOR_OFF:
        {
            break;
        }
        case HOME:
        {
            sharedMemory->controlState = STATE_HOME_READY;
            break;
        }
        case SETPARAMS:
        {
            break;
        }
        case CUBIC_CONTROL:
        {
            sharedMemory->controlState = STATE_CUBIC_READY;
            break;
        }
        case COS_CONTROL:
        {
            sharedMemory->controlState = STATE_COS_READY;
            break;
        }
        case LOAD_MODEL:
        {
            sharedMemory->torchState = TORCH_LOAD_MODEL;
            break;
        }
        case ONLINE_LEARNING:
        {
            if(mbOnlineLearning)
            {
                sharedMemory->torchState = TORCH_ONLINE_LEARNING;
            }
            else
            {
                sharedMemory->torchState = TORCH_ESTIMATION;
            }
            mbOnlineLearning = !mbOnlineLearning;
            break;
        }
        case SAVE_DATA:
        {
            writeToCSVfile();
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