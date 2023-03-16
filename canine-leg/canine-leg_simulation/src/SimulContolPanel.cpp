//
// Created by hs on 22. 10. 27.
//

#include <canine-leg_simulation/SimulControlPanel.hpp>

extern pUI_COMMAND sharedCommand;
extern pSHM sharedMemory;

SimulControlPanel::SimulControlPanel(raisim::World* world, raisim::ArticulatedSystem* robot)
    : mWorld(world)
    , mRobot(robot)
    , mIteration(0)
{
    mTorque.setZero();
}

void SimulControlPanel::ControllerFunction()
{
    sharedMemory->localTime = mIteration * CONTROL_dT;
    mIteration++;
    switch (sharedMemory->controlState)
    {
    case STATE_CONTROL_STOP:
    {
        break;
    }
    case STATE_HOME_READY:
    {
        updateStates();
        PDcontrol.InitHomeTrajectory();
        sharedMemory->controlState = STATE_HOME_CONTROL;
        sharedMemory->visualState = VISUAL_UPDATE_RAISIM;
        break;
    }
    case STATE_HOME_CONTROL:
    {
        PDcontrol.DoHomeControl();
        break;
    }
    case STATE_CUBIC_READY:
    {
        PDcontrol.InitCubicTrajectory();
        sharedMemory->controlState = STATE_CUBIC_CONTROL;
        break;
    }
    case STATE_CUBIC_CONTROL:
    {
        PDcontrol.DoCubicControl();
        break;
    }
    case STATE_COS_READY:
    {
        PDcontrol.InitCosTrajectory();
        sharedMemory->controlState = STATE_COS_CONTROL;
        break;
    }
    case STATE_COS_CONTROL:
    {
        PDcontrol.DoCosControl();
        break;
    }
    default:
        break;
    }
    if(sharedMemory->visualState == VISUAL_UPDATE_RAISIM)
    {
        integrateSimul();
    }
//    if (sharedMemory->measuredGRF < 0.1)
//    {
//        sharedMemory->estimatedGRFMLP = 0.0;
//    }
}

void SimulControlPanel::integrateSimul()
{
    for (int idx = 0; idx < MOTOR_NUM; idx++)
    {
        mTorque[idx + 1] = sharedMemory->motorDesiredTorque[idx];
    }
    mRobot->setGeneralizedForce(mTorque);
    mWorld->integrate();
    updateStates();
}

void SimulControlPanel::updateStates()
{
//    double pastHipVerticalVelocity;
//    pastHipVerticalVelocity = sharedMemory->hipVerticalVelocity;
    sharedMemory->hipVerticalPosition = mRobot->getGeneralizedCoordinate()[0];
    sharedMemory->motorPosition[0] = mRobot->getGeneralizedCoordinate()[1];
    sharedMemory->motorPosition[1] = mRobot->getGeneralizedCoordinate()[2];
    sharedMemory->hipVerticalVelocity = mRobot->getGeneralizedVelocity()[0];
//    sharedMemory->hipVerticalAcceleration = (sharedMemory->hipVerticalVelocity - pastHipVerticalVelocity) / CONTROL_dT;
    sharedMemory->motorVelocity[0] = mRobot->getGeneralizedVelocity()[1];
    sharedMemory->motorVelocity[1] = mRobot->getGeneralizedVelocity()[2];

    if (&(mRobot->getContacts()[0]) != nullptr)
    {
        sharedMemory->measuredGRF = mRobot->getContacts()[0].getImpulse()[2] / CONTROL_dT;
    }
}