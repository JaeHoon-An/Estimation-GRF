cmake_minimum_required(VERSION 3.5)
project(canine-leg_util LANGUAGES CXX)

find_package(Eigen3 REQUIRED HINTS ${Eigen3_HINT})

include_directories(
        include
        ${EIGEN3_INCLUDE_DIRS}
)

add_library(canine-leg_util
        SHARED
        src/CosTrajectoryGenerator.cpp
        src/JointPDController.cpp
        src/LoadCell.cpp
        )
