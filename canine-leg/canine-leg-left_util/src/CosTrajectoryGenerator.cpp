//
// Created by jaehoon on 23. 3. 3.
//

#include <canine-leg-left_util/CosTrajectoryGenerator.hpp>

void CosTrajectoryGenerator::updateTrajectory(double currentPosition, double currentTime, double amplitude, double frequency){
    mReferencePose = currentPosition;
    mReferenceTime = currentTime;
    mAmplitude = amplitude;
    mFrequency = frequency;
}

double CosTrajectoryGenerator::getPositionTrajectory(double currentTime) {
    double time = (currentTime - mReferenceTime);
    return mAmplitude*cos(2*PI*mFrequency*time)+mReferencePose-mAmplitude;
}

double CosTrajectoryGenerator::getVelocityTrajectory(double currentTime) {
    double time = (currentTime - mReferenceTime);
    return -2*PI*mFrequency*mAmplitude*sin(2*PI*mFrequency*time);
}