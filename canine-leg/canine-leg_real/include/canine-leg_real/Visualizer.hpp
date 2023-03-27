//
// Created by hs on 22. 10. 28.
//

#ifndef RAISIM_SIMULVISUALIZER_HPP
#define RAISIM_SIMULVISUALIZER_HPP

#include <raisim/World.hpp>
#include <raisim/RaisimServer.hpp>
#include <canine-leg_util/SharedMemory.hpp>

class Visualizer
{
public:
    Visualizer(raisim::World* world,
                    raisim::ArticulatedSystem* robot,
                    raisim::RaisimServer* server);
    ~Visualizer();
    void UpdateVisual();

private:
    void initRobotPose();

private:
    raisim::World* mWorld;
    raisim::ArticulatedSystem* mRobot;
    raisim::RaisimServer* mServer;
};

#endif //RAISIM_SIMULVISUALIZER_HPP
