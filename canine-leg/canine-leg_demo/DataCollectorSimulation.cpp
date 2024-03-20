//
// Created by jaehoon on 23. 3. 2.
//

#include <iostream>
#include "raisim/RaisimServer.hpp"
#include <canine-leg_util/CosTrajectoryGenerator.hpp>
#include <canine-leg_util/SharedMemory.hpp>

pSHM sharedMemory;

//std::string urdfPath = std::string(URDF_RSC_DIR) + "/canine_leg_left_v4/canine_leg_left_v4.urdf";
std::string urdfPath = std::string(URDF_RSC_DIR) + "/canine_leg_left/canine_leg_left_V2.urdf";
std::string name = "canine-leg";

raisim::World world;
auto ground = world.addGround(0, "gnd");
auto robot = world.addArticulatedSystem(urdfPath);
raisim::RaisimServer server(&world);

CosTrajectoryGenerator mTrajectoryGenerator;

raisim::VecDyn mPosition = raisim::VecDyn(3);
raisim::VecDyn mVelocity = raisim::VecDyn(3);
raisim::VecDyn mDesiredPosition = raisim::VecDyn(3);
raisim::VecDyn mDesiredVelocity = raisim::VecDyn(3);
raisim::VecDyn mPositionError = raisim::VecDyn(3);
raisim::VecDyn mVelocityError = raisim::VecDyn(3);

raisim::VecDyn mTorque = raisim::VecDyn(3);
Eigen::VectorXd mPastDesiredPosition = Eigen::VectorXd(3);
Eigen::VectorXd mInitialJointPosition(robot->getGeneralizedCoordinateDim());
Eigen::VectorXd mInitialJointVelocity(robot->getGeneralizedVelocityDim());

Eigen::VectorXd mGRFtrue = Eigen::VectorXd(3);
int mDataIdx = 0;
int maxIdx = 4000000;
Eigen::MatrixXd mStates = Eigen::MatrixXd(maxIdx, 15); // 3time-series of joint pos & vel  + current torque : 12 + 2 = 14 (input dim.)
Eigen::MatrixXd mPositionBuffer = Eigen::MatrixXd(41, 2);
Eigen::MatrixXd mVelocityBuffer = Eigen::MatrixXd(41, 2);

double mLocalTime = 0;
double dT = 0.001;
double Kp = 70.0;
double Kd = 2.5;
double mTorqueLimit = 20.0;

bool isFirstGenTraj = true;
bool stopFlag = false;
bool collectFlag = false;
bool realTimeVisual = true;
double mSetPoint = 0.23;

double pastKneeTorque = 0;

void resetJointState(double initialHeight)
{
    mInitialJointPosition.setZero();
    mInitialJointVelocity.setZero();
    mInitialJointPosition << initialHeight, acos(initialHeight / 0.46), -2.0 * acos(initialHeight / 0.46);
//    mInitialJointPosition << 1.0, 0.0, 0.0;
    robot->setGeneralizedCoordinate(mInitialJointPosition);
    robot->setGeneralizedVelocity(mInitialJointVelocity);
}

void reset(double initialHeight)
{
    isFirstGenTraj = true;
    stopFlag = false;
    collectFlag = false;
    resetJointState(initialHeight);
    mPositionBuffer.setZero();
    mVelocityBuffer.setZero();
    mPosition = robot->getGeneralizedCoordinate();
    mVelocity = robot->getGeneralizedVelocity();
    mDesiredPosition = mPosition;
}

void updateState()
{
    mPosition = robot->getGeneralizedCoordinate();
    mVelocity = robot->getGeneralizedVelocity();
}

void setSineTrajectory()
{
    mDesiredPosition[0] = mTrajectoryGenerator.getPositionTrajectory(mLocalTime);
    mPastDesiredPosition = mDesiredPosition.e();
    mDesiredPosition[1] = acos(mDesiredPosition[0] / 0.46);
    mDesiredPosition[2] = -2.0 * mDesiredPosition[1];

    if (isFirstGenTraj)
    {
        mPastDesiredPosition[0] = mDesiredPosition[0];
        mPastDesiredPosition[1] = mDesiredPosition[1];
        mPastDesiredPosition[2] = mDesiredPosition[2];
        isFirstGenTraj = false;
    }


    mDesiredVelocity[0] = mTrajectoryGenerator.getVelocityTrajectory(mLocalTime);
    mDesiredVelocity[1] = (mDesiredPosition[1] - mPastDesiredPosition[1]) / dT;
    mDesiredVelocity[2] = (mDesiredPosition[2] - mPastDesiredPosition[2]) / dT;
}

void pdControl()
{
    for (int i = 0; i < 3; i++)
    {
        mPositionError[i] = mDesiredPosition[i] - mPosition[i];
        mVelocityError[i] = mDesiredVelocity[i] - mVelocity[i];
        mTorque[i] = Kp * mPositionError[i] + Kd * mVelocityError[i];
    }
    mTorque[0] = 0;
}

/**
 * doControl follows these steps : \n
 * 1. update states \n
 * 2. set trajectories \n
 * 3. compute torques
 */
void doSineControl()
{
    updateState();
    setSineTrajectory();
    pdControl();
    for (int i = 0; i < 3; i++)
    {
        if (mTorque[i] > mTorqueLimit)
        {
            mTorque[i] = mTorqueLimit;
        }
        else if (mTorque[i] < -mTorqueLimit)
        {
            mTorque[i] = -mTorqueLimit;
        }
    }
    robot->setGeneralizedForce(mTorque);
}

void setPointTrajectory(double desiredHeight)
{
    mDesiredPosition[0] = desiredHeight;
    mDesiredPosition[1] = acos(mDesiredPosition[0] / 0.46);
    mDesiredPosition[2] = -2.0 * mDesiredPosition[1];
    mDesiredVelocity.setZero();
}

void doSetPointControl()
{
    updateState();
    setPointTrajectory(mSetPoint);
    pdControl();
    for (int i = 0; i < 3; i++)
    {
        if (mTorque[i] > mTorqueLimit)
        {
            mTorque[i] = mTorqueLimit;
        }
        else if (mTorque[i] < -mTorqueLimit)
        {
            mTorque[i] = -mTorqueLimit;
        }
    }
    robot->setGeneralizedForce(mTorque);
}

/**
 * Save GRF in mGRFtrue : [GRFx, GRFy, GRFz]
 */
void getGRF()
{
    auto contact = robot->getContacts();
    if (&(contact[0]) != nullptr)
    {
        mGRFtrue = contact[0].getImpulse().e();
        mGRFtrue.operator/=(dT);
    }
}

void writeToCSVfile()
{
    std::string name2 = "../../datasets/training/GRFDatasets_Leg_v2_1.csv";
    std::ofstream file2(name2.c_str());
    for (int i = 0; i < mDataIdx; i++)
    {
        for (int j = 0; j < mStates.cols(); j++)
        {
            std::string str = std::to_string(mStates(i, j));
            if (j + 1 == mStates.cols())
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

void updateBuffer()
{
    for (int i = 40; i > 0; i--)
    {
        mPositionBuffer(i, 0) = mPositionBuffer(i - 1, 0);
        mPositionBuffer(i, 1) = mPositionBuffer(i - 1, 1);
        mVelocityBuffer(i, 0) = mVelocityBuffer(i - 1, 0);
        mVelocityBuffer(i, 1) = mVelocityBuffer(i - 1, 1);
    }
    mPositionBuffer(0, 0) = mPosition[1];
    mPositionBuffer(0, 1) = mPosition[2];
    mVelocityBuffer(0, 0) = mVelocity[1];
    mVelocityBuffer(0, 1) = mVelocity[2];
}

void collectData()
{
    if (mGRFtrue[2] == 0)
    {
        std::cout << "[DATA COLLECTOR] Zero GRF is occurred." << std::endl;
        stopFlag = true;
    }
    else if (mGRFtrue[2] > 80)
    {
        std::cout << "[DATA COLLECTOR] Raisim error 1 is occurred." << std::endl;
        stopFlag = true;
    }
    else if (mPosition[0] < 0.03)
    {
        std::cout << "[DATA COLLECTOR] Base contact is occurred." << std::endl;
        stopFlag = true;
    }
    else if (isnan(mPosition[0] + mPosition[1] + mPosition[2] + mVelocity[0] + mVelocity[1] + mVelocity[2] + mGRFtrue[2] + mTorque[1] + mTorque[2]))
    {
        std::cout << "[DATA COLLECTOR] nan is occurred." << std::endl;
        stopFlag = true;
    }
    else if (collectFlag == true)
    {
        if(abs(pastKneeTorque - mTorque[2])>1.0)
        {
            std::cout << "[DATA COLLECTOR] Raisim error 2 is occurred." << std::endl;
            stopFlag = true;
        }
        else
        {
            mStates(mDataIdx, 0) = mPositionBuffer(0, 0);
            mStates(mDataIdx, 1) = mPositionBuffer(20, 0);
            mStates(mDataIdx, 2) = mPositionBuffer(40, 0);
            mStates(mDataIdx, 3) = mPositionBuffer(0, 1);
            mStates(mDataIdx, 4) = mPositionBuffer(20, 1);
            mStates(mDataIdx, 5) = mPositionBuffer(40, 1);
            mStates(mDataIdx, 6) = mVelocityBuffer(0, 0);
            mStates(mDataIdx, 7) = mVelocityBuffer(20, 0);
            mStates(mDataIdx, 8) = mVelocityBuffer(40, 0);
            mStates(mDataIdx, 9) = mVelocityBuffer(0, 1);
            mStates(mDataIdx, 10) = mVelocityBuffer(20, 1);
            mStates(mDataIdx, 11) = mVelocityBuffer(40, 1);
            mStates(mDataIdx, 12) = mTorque[1];
            mStates(mDataIdx, 13) = mTorque[2];
            mStates(mDataIdx, 14) = mGRFtrue[2];
            mDataIdx++;
        }
    }
    pastKneeTorque = mTorque[2];
}

void doTest(double sineOffset, double sineAmplitude, double sineFrequency)
{
    std::cout << "[SYSTEM] offset, amplitude, frequency : " << sineOffset << ", " << sineAmplitude << ", " << sineFrequency << std::endl;
    mSetPoint = sineOffset;
    reset(mSetPoint);
    for (int i = 0; i < 2000; i++)
    {
        doSetPointControl();
        updateBuffer();
        world.integrate();
        getGRF();
        if (realTimeVisual)
        {
            usleep(1000);
        }
    }
    mTrajectoryGenerator.updateTrajectory(sineOffset, mLocalTime, sineAmplitude, sineFrequency);

    int iteration = 0;
    int period = int(1 / sineFrequency / dT);
    while (true)
    {
        doSineControl();
        updateBuffer();
        getGRF();
        collectData();
        world.integrate();
        iteration++;
        mLocalTime = iteration * world.getTimeStep();
        if (iteration == period)
        {
            collectFlag = true;
        }

        if ((iteration == 2 * period) || stopFlag)
        {
            std::cout << "[SYSTEM] iteration : " << iteration << std::endl;
            std::cout << "[SYSTEM] data index : " << mDataIdx << std::endl << std::endl;
            break;
        }
        if (realTimeVisual)
        {
            usleep(1000);
        }
    }
}

int main()
{
    sharedMemory = (pSHM)malloc(sizeof(SHM));
    world.setTimeStep(dT);
    mStates.setZero();
    server.launchServer();
    reset(mSetPoint);
    sleep(2);
    realTimeVisual = false;
    double sineOffset;
    double sineAmplitude;
    double sineFrequency;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            for (int k = 0; k < 40; k++)
            {
//                sineOffset = 0.1 + 0.035 * i;
//                sineAmplitude = 0.05 + 0.04 * j;
//                sineFrequency = 0.1 + 0.05 * k;

                sineOffset = 0.1 + 0.035 * i;
                sineAmplitude = 0.05 + 0.015 * j;
                sineFrequency = 0.1 + 0.05 * k;
                doTest(sineOffset, sineAmplitude, sineFrequency);
            }
        }
    }
    writeToCSVfile();
    server.killServer();
}