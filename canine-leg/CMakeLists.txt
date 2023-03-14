cmake_minimum_required(VERSION 3.5)
project(camel-canine-leg-left LANGUAGES CXX)

find_package(Qt5 CONFIG REQUIRED COMPONENTS Widgets)
find_package(Eigen3 REQUIRED HINTS ${Eigen3_HINT})
find_package(camel-tools-thread REQUIRED)
find_package(camel-tools-sensor REQUIRED)

add_subdirectory(canine-leg_util)

include_directories(
        ${EIGEN3_INCLUDE_DIRS}
        ${canine-leg_util_SOURCE_DIR}/include
)

add_executable(canine-leg-left-data canine-leg_demo/DataCollector.cpp)
target_link_libraries(canine-leg-left-data
        PUBLIC
        Qt5::Widgets
        raisim
        pthread
        PRIVATE
        canine-leg_util
        )