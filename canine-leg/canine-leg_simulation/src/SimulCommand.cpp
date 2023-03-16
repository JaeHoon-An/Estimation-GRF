//
// Created by hs on 22. 10. 28.
//

#include <canine-leg_simulation/SimulCommand.hpp>

extern pUI_COMMAND sharedCommand;
extern pSHM sharedMemory;

void SimulCommand::commandFunction()
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
            sharedMemory->torchState = TORCH_ONLINE_LEARNING;
            break;
        }
        default:
            break;
        }
    }
}