//
// Created by jh on 22. 11. 3.
//

#include <ControlUtils/GRFEstimatorMLP.hpp>

extern pSHM sharedMemory;

GRFEstimatorMLP::GRFEstimatorMLP()
{

}

void GRFEstimatorMLP::Estimate()
{
    sharedMemory->estimatedGRFMLP = 0.0;
}