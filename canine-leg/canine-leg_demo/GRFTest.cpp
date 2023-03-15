//
// Created by jaehoon on 23. 3. 15.
//

#include <canine-leg_util/LoadCell.hpp>
#include <camel-tools/LPF.hpp>
#include <iostream>

LoadCell loadcell;
LPF lpf(0.001, 50);

int main()
{
    double sensoredWeight;
    double filteredWeight;
    while(true)
    {
        loadcell.ReadData();
        sensoredWeight = loadcell.GetSensoredWeight();
        filteredWeight = lpf.GetFilteredVar(sensoredWeight);
        std::cout<<"RAW weight : "<<sensoredWeight<<std::endl;
        std::cout<<"Filtered weight : "<<filteredWeight<<std::endl;
//        usleep(1000);
    }
}