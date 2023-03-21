//
// Created by hs on 22. 10. 27.
//

#ifndef RAISIM_SIMULCONTROLPANEL_HPP
#define RAISIM_SIMULCONTROLPANEL_HPP

#include <stdint.h>
#include <raisim/World.hpp>
#include <raisim/math.hpp>
#include <canine-leg_util/SharedMemory.hpp>
#include <canine-leg_simulation/JointPDController.hpp>

class SimulControlPanel
{
public:
    SimulControlPanel(raisim::World* world, raisim::ArticulatedSystem* robot);

    void ControllerFunction();

private:
    void integrateSimul();
    void updateStates();
    void clearBuffer();
    void updateBuffer();
    void updateNetInputs();

private:
    raisim::World* mWorld;
    raisim::ArticulatedSystem* mRobot;
    JointPDController PDcontrol;
    uint64_t mIteration;
    double mBufferPosition [BUFFER_SIZE][MOTOR_NUM];
    double mBufferVelocity [BUFFER_SIZE][MOTOR_NUM];
    raisim::VecDyn mTorque = raisim::VecDyn(3);
};

#endif //RAISIM_SIMULCONTROLPANEL_HPP
