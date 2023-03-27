cmake_minimum_required(VERSION 3.5)
project(canine-leg_real LANGUAGES CXX)

find_package(camel-tools-thread REQUIRED)
find_package(camel-tools-trajectory REQUIRED)

include_directories(
        include
        ${EIGEN3_INCLUDE_DIRS}
        ${canine-leg_util_SOURCE_DIR}/include
)

add_library(${PROJECT_NAME}
        STATIC
        src/PhysicalWorldMain.cpp
        src/ControlPanel.cpp
        src/Visualizer.cpp
        src/Command.cpp)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        raisim
        camel-tools-thread
        canine-leg_util
        camel-tools-trajectory
        PRIVATE
        pthread
)