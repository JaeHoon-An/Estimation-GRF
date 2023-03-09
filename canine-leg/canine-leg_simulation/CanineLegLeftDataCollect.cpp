//
// Created by jaehoon on 23. 3. 2.
//

#include <iostream>
#include "raisim/RaisimServer.hpp"
#include <canine-leg_util/CosTrajectoryGenerator.hpp>

std::string urdfPath = std::string(URDF_RSC_DIR) + "/canine_leg_left/canine_leg_left.urdf";
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
Eigen::VectorXd mPastDesiredPosition = Eigen::VectorXd (3);
Eigen::VectorXd mInitialJointPosition(robot->getGeneralizedCoordinateDim());
Eigen::VectorXd mInitialJointVelocity(robot->getGeneralizedVelocityDim());

Eigen::VectorXd mGRFtrue = Eigen::VectorXd (3);

int maxIdx = 1000;
Eigen::MatrixXd mStates = Eigen::MatrixXd(maxIdx,7);

double mLocalTime = 0;
double dT = 0.001;
double Kp = 100.0;
double Kd = 5.0;
double mTorqueLimit = 20.0;

bool isFirstGenTraj = true;

void resetJointState()
{
    mInitialJointPosition.setZero();
    mInitialJointVelocity.setZero();
    mInitialJointPosition<<0.23, 1.04719755, -2.0943951;
    robot->setGeneralizedCoordinate(mInitialJointPosition);
    robot->setGeneralizedVelocity(mInitialJointVelocity);
}

void reset()
{
    resetJointState();
    mPosition = robot->getGeneralizedCoordinate();
    mVelocity = robot->getGeneralizedVelocity();
    mDesiredPosition = mPosition;
}

void updateState()
{
    mPosition = robot->getGeneralizedCoordinate();
    mVelocity = robot->getGeneralizedVelocity();
}

void setTrajectory()
{
    mDesiredPosition[0] = mTrajectoryGenerator.getPositionTrajectory(mLocalTime);
    mPastDesiredPosition = mDesiredPosition.e();
    mDesiredPosition[1] = acos(mDesiredPosition[0] / 0.46);
    mDesiredPosition[2] = -2.0 * mDesiredPosition[1];

    if(isFirstGenTraj)
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
    for(int i = 0 ; i < 3 ; i ++)
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
void doControl()
{
    updateState();
    setTrajectory();
    pdControl();
    for(int i = 0 ; i < 3 ; i++)
    {
        if(mTorque[i] > mTorqueLimit)
        {
            mTorque[i] = mTorqueLimit;
        }
        else if(mTorque[i] < -mTorqueLimit)
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
    std::string name2 = "../../data/GRFTrainingData.csv";
    std::ofstream file2(name2.c_str());
    for(int  i = 0; i < mStates.rows(); i++)
    {
        for(int j = 0; j < mStates.cols(); j++)
        {
            std::string str = std::to_string(mStates(i,j));
            if(j+1 == mStates.cols()){
                file2<<str;
            }else{
                file2<<str<<',';
            }
        }
        file2<<'\n';
    }
    std::cout<<"Data is saved."<<std::endl;
}

void collectData(int idx)
{
    mStates(idx, 0) = mPosition[1];
    mStates(idx, 1) = mPosition[2];
    mStates(idx, 2) = mVelocity[1];
    mStates(idx, 3) = mVelocity[2];
    mStates(idx, 4) = mTorque[1];
    mStates(idx, 5) = mTorque[2];
    mStates(idx, 6) = mGRFtrue[2];
}

int main()
{
    world.setTimeStep(dT);
    reset();
    server.launchServer();
    sleep(2);

    mTrajectoryGenerator.updateTrajectory(mPosition[0], mLocalTime,0.05,1);

    int iteration = 0;
    while(true)
    {
        doControl();
        getGRF();
        collectData(iteration);
        world.integrate();
        usleep(1000);
        iteration++;
        mLocalTime = iteration * world.getTimeStep();
        if(iteration == maxIdx)
        {
            break;
        }
    }
    writeToCSVfile();
    server.killServer();
}