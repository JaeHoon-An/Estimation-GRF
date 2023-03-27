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
        case SIM_NO_ACT:
        {
            break;
        }
        case SIM_MOTOR_ON:
        {
            break;
        }
        case SIM_MOTOR_OFF:
        {
            break;
        }
        case SIM_HOME:
        {
            sharedMemory->controlState = STATE_HOME_READY;
            break;
        }
        case SIM_SETPARAMS:
        {
            break;
        }
        case SIM_CUBIC_CONTROL:
        {
            sharedMemory->controlState = STATE_CUBIC_READY;
            break;
        }
        case SIM_COS_CONTROL:
        {
            sharedMemory->controlState = STATE_COS_READY;
            break;
        }
        case SIM_LOAD_MODEL:
        {
            sharedMemory->torchState = TORCH_LOAD_MODEL;
            break;
        }
        case SIM_ONLINE_LEARNING:
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
        case SIM_SAVE_DATA:
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
    name2.append("GRFDatasetsSimTransfer.csv");
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