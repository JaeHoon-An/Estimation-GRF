//
// Created by jaehoon on 23. 3. 3.
//

#ifndef RAISIM_COSTRAJECTORYGENERATOR_HPP
#define RAISIM_COSTRAJECTORYGENERATOR_HPP

#include "SharedMemory.hpp"
#include "math.h"

class CosTrajectoryGenerator
{
public:
    void updateTrajectory(double currentPosition, double currentTime, double amplitude, double frequency);
    double getPositionTrajectory(double currentTime);
    double getVelocityTrajectory(double currentTime);

private:
    double mReferencePose;
    double mReferenceTime;
    double mAmplitude;
    double mFrequency;
};


#endif //RAISIM_COSTRAJECTORYGENERATOR_HPP
