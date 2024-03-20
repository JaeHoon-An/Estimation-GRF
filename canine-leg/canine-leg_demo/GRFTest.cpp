//
// Created by jaehoon on 23. 3. 15.
//

#include <canine-leg_util/LoadCell.hpp>
#include <canine-leg_util/SharedMemory.hpp>
#include <camel-tools/LPF.hpp>
#include <camel-tools/ThreadGenerator.hpp>
#include <iostream>

LoadCell loadcell;
struct timespec TIME_1;
struct timespec TIME_2;
pSHM sharedMemory;
LPF lpf(0.001, 15);

int main()
{
    sharedMemory = (pSHM)malloc(sizeof(SHM));

    double sensoredWeight;
    double filteredWeight;
    uint64_t sumedRawData;
    int iteration = 0;
    sumedRawData = 0.0;
    double sumedWeight = 0.0;

    while(true)
    {
        clock_gettime(CLOCK_REALTIME, &TIME_1);
        loadcell.ReadData();
        usleep(650);
        clock_gettime(CLOCK_REALTIME, &TIME_2);
        std::cout << "one estimation time : "<<timediff_us(&TIME_1, &TIME_2) << std::endl;
        sensoredWeight = loadcell.GetSensoredWeight();
        filteredWeight = lpf.GetFilteredVar(sensoredWeight);
//        std::cout<<"RAW weight : "<<sensoredWeight<<std::endl;
//        std::cout<<"RAW force : "<<loadcell.GetSensoredForce()<<std::endl;
        std::cout<<"Filtered weight : "<<filteredWeight<<std::endl;
//        std::cout<<"RAW DATA : "<<loadcell.GetRawData()<<std::endl;
//        sumedRawData += loadcell.GetRawData();
        sumedWeight += sensoredWeight;
        iteration++;
//        if(iteration == 10000)
//        {
//            break;
//        }
    }
    std::cout<<sumedWeight/10000<<std::endl;
}