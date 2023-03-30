//
// Created by hs on 22. 10. 27.
//

#ifndef RAISIM_SIMULCONTROLPANEL_HPP
#define RAISIM_SIMULCONTROLPANEL_HPP

#include <stdint.h>
#include <raisim/World.hpp>
#include <raisim/math.hpp>
#include <canine-leg_util/SharedMemory.hpp>
#include <canine-leg_util/JointPDController.hpp>

class ControlPanel
{
public:
    ControlPanel();

    void ControllerFunction();

private:
    void clearBuffer();
    void updateBuffer();
    void updateNetInputs();

private:
    JointPDController PDcontrol;
    uint64_t mIteration;
    int mMotionTableIdx;
    double mBufferPosition [NET_INPUT_BUFFER_SIZE][MOTOR_NUM];
    double mBufferVelocity [NET_INPUT_BUFFER_SIZE][MOTOR_NUM];
    Eigen::MatrixXd mGRFBuffer = Eigen::MatrixXd(10, 1);
    double mReferenceTime;
};

#endif //RAISIM_SIMULCONTROLPANEL_HPP
