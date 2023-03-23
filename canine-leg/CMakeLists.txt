cmake_minimum_required(VERSION 3.5)
project(camel-canine-leg-left LANGUAGES CXX)

find_package(Qt5 CONFIG REQUIRED COMPONENTS Widgets)
find_package(Torch REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Eigen3 REQUIRED HINTS ${Eigen3_HINT})
find_package(camel-tools-thread REQUIRED)
find_package(camel-tools-sensor REQUIRED)
find_package(camel-tools-filter REQUIRED)

add_subdirectory(canine-leg_util)
add_subdirectory(canine-leg_simulation)

include_directories(
        ${EIGEN3_INCLUDE_DIRS}
        ${canine-leg_util_SOURCE_DIR}/include
        ${canine-leg_simulation_SOURCE_DIR}/include
        canine-leg_GUI
)

add_executable(canine-leg-left-data canine-leg_demo/DataCollectorSimulation.cpp)
target_link_libraries(canine-leg-left-data
        PUBLIC
        Qt5::Widgets
        raisim
        pthread
        PRIVATE
        canine-leg_util
        )

add_executable(canine-leg-left-simulation
        canine-leg_demo/Simulation.cpp
        canine-leg_demo/TorchModelManager.cpp
        canine-leg_GUI/SimulationGUI/mainwindow.cpp
        canine-leg_GUI/util/qcustomplot.cpp)

target_link_libraries(canine-leg-left-simulation
        PUBLIC
        "${TORCH_LIBRARIES}"
        Qt5::Widgets
        raisim
        pthread
        PRIVATE
        canine-leg_util
        canine-leg_simulation
        camel-tools-filter
        )

add_executable(GRFtest canine-leg_demo/GRFTest.cpp)
target_link_libraries(GRFtest
        canine-leg_util
        camel-tools-filter
        )

set_property(TARGET canine-leg-left-simulation PROPERTY CXX_STANDARD 14)