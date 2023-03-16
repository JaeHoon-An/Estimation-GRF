CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(Estimation-GRF)

list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/Library/libtorch")
ADD_COMPILE_DEFINITIONS (DATASET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/datasets/training/")
ADD_COMPILE_DEFINITIONS (VALIDSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/datasets/validation/")
ADD_COMPILE_DEFINITIONS (SIMULRESULT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/datasets/experiments/simulation/")
ADD_COMPILE_DEFINITIONS (RESULT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/datasets/experiments/real/")
ADD_COMPILE_DEFINITIONS (MODEL_DIR="${CMAKE_CURRENT_SOURCE_DIR}/model/")
ADD_COMPILE_DEFINITIONS (URDF_RSC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/rsc/urdf/")

set(RAISIM_PATH "$ENV{HOME}/Library/raisimLib/raisim/linux")
include_directories(${RAISIM_PATH}/include)
link_directories(${RAISIM_PATH}/lib)


add_subdirectory(canine-leg)
add_subdirectory(offlineLearner)