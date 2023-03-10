CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(Estimation-GRF)

list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/Library/libtorch")
ADD_COMPILE_DEFINITIONS (DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/")
ADD_COMPILE_DEFINITIONS (MODEL_DIR="${CMAKE_CURRENT_SOURCE_DIR}/model/")
set(RAISIM_PATH "$ENV{HOME}/Library/raisimLib/raisim/linux")
include_directories(${RAISIM_PATH}/include)
link_directories(${RAISIM_PATH}/lib)

ADD_COMPILE_DEFINITIONS (URDF_RSC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/rsc/urdf/")

add_subdirectory(canine-leg)
add_subdirectory(offlineLearner)