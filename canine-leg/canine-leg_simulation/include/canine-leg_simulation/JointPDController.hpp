//
// Created by camel on 22. 9. 21.
//

#ifndef RAISIM_JOINTPDCONTROLLER_H
#define RAISIM_JOINTPDCONTROLLER_H

#include <canine-leg_util/SharedMemory.hpp>
#include <canine-leg_util/CosTrajectoryGenerator.hpp>
#include <camel-tools/CubicTrajectoryGenerator.hpp>


class JointPDController
{
public:
    JointPDController();

    void DoHomeControl();
    void DoCubicControl();
    void DoCosControl();
    void InitHomeTrajectory();
    void InitCubicTrajectory();
    void InitCosTrajectory();
    void SetControlInput();

private:
    void setHomeTrajectory();
    void setCubicTrajectory();
    void setCosTrajectory();
    void solveIK();
    void computeControlInput();


private:
    CubicTrajectoryGenerator mCubicTrajectoryGen[MOTOR_NUM];
    CubicTrajectoryGenerator mCubicTrajectoryGenHipVertical;
    CosTrajectoryGenerator mCosTrajectoryGenerator;
    bool mIsFirstGenTraj;
    double mPastDesiredPosition[MOTOR_NUM];
    double mDesiredPosition[MOTOR_NUM];
    double mDesiredVelocity[MOTOR_NUM];
    double mTorque[MOTOR_NUM];
    double mTorqueLimit[MOTOR_NUM];

};


#endif //RAISIM_JOINTPDCONTROLLER_H
