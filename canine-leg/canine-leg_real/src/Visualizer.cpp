//
// Created by hs on 22. 10. 28.
//

#include <canine-leg_real/Visualizer.hpp>
#include <canine-leg_util/CustomMath.hpp>

extern pSHM sharedMemory;

Visualizer::Visualizer(raisim::World* world, raisim::ArticulatedSystem* robot, raisim::RaisimServer* server)
    : mWorld(world)
    , mRobot(robot)
    , mServer(server)
{
    mWorld->setGravity({ 0.0, 0.0, -9.81 });
    mWorld->setTimeStep(CONTROL_dT);
    mWorld->addGround();
    mRobot->setName("Canine-leg");
    initRobotPose();
}

Visualizer::~Visualizer()
{
    mServer->killServer();
}

void Visualizer::UpdateVisual()
{
    Eigen::VectorXd jointPosition(mRobot->getGeneralizedCoordinateDim());
    Eigen::VectorXd jointVelocity(mRobot->getGeneralizedVelocityDim());
    jointPosition.setZero();
    jointVelocity.setZero();

    jointPosition[0] = sharedMemory->hipVerticalPosition;
    jointPosition[1] = sharedMemory->motorPosition[HIP_IDX];
    jointPosition[2] = sharedMemory->motorPosition[KNEE_IDX];

    jointVelocity[0] = sharedMemory->hipVerticalVelocity;
    jointVelocity[1] = sharedMemory->motorVelocity[HIP_IDX];
    jointVelocity[2] = sharedMemory->motorVelocity[KNEE_IDX];

    mRobot->setGeneralizedCoordinate(jointPosition);
    mRobot->setGeneralizedVelocity(jointVelocity);
}

void Visualizer::initRobotPose()
{
    Eigen::VectorXd initialJointPosition(mRobot->getGeneralizedCoordinateDim());
    Eigen::VectorXd initialJointVelocity(mRobot->getGeneralizedVelocityDim());
    initialJointPosition.setZero();
    initialJointVelocity.setZero();

    initialJointPosition[0] = 0.09167;
    initialJointPosition[1] = 80 * D2R;
    initialJointPosition[2] = -160 * D2R;

    mRobot->setGeneralizedCoordinate(initialJointPosition);
    mRobot->setGeneralizedForce(Eigen::VectorXd::Zero(mRobot->getDOF()));
    mRobot->setGeneralizedVelocity(initialJointVelocity);
}
