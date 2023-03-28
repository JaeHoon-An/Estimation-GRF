//
// Created by hs on 22. 10. 28.
//

#ifndef RAISIM_SIMULCOMMAND_HPP
#define RAISIM_SIMULCOMMAND_HPP

#include <fstream>
#include <iostream>
#include <canine-leg_util/SharedMemory.hpp>
#include <Eigen/Eigen>

class Command
{
public:
    Command();
    void commandFunction();

private:
    void saveResult();
    void writeToCSVfile();
};

#endif //RAISIM_SIMULCOMMAND_HPP
