//
// Created by jaehoon on 23. 3. 17.
//

#include "TorchModelManager.hpp"

extern pSHM sharedMemory;

TorchModelManager::TorchModelManager()
{
    mANN = std::make_shared<Net>(NET_INPUT_SIZE, NET_OUTPUT_SIZE);
}

void TorchModelManager::torchFunction()
{
    switch (sharedMemory->torchState)
    {
    case TORCH_NO_ACT:
    {
        break;
    }
    case TORCH_LOAD_MODEL:
    {
        loadModel();
        sharedMemory->torchState = TORCH_NO_ACT;
        break;
    }
    case TORCH_ESTIMATION:
    {
        estimation();
        break;
    }
    case TORCH_ONLINE_LEARNING:
    {
        onlineLearning();
        break;
    }
    default:
        break;
    }
}

void TorchModelManager::loadModel()
{
    std::string modelPath;
    modelPath = *(sharedMemory->modelName);
    std::ifstream ifs(modelPath);
    try
    {
        if (!ifs.is_open())
        {
            throw modelPath;
        }
        torch::load(mANN, modelPath);
        std::cout << "[MODEL MANAGER] : " << modelPath << " is loaded." << std::endl << std::endl;
    }
    catch (std::string path)
    {
        perror("[MODEL MANAGER] Model is not loaded.");
        std::cout << "[MODEL MANAGER] Given path : " << path << std::endl << std::endl;
    }
    ifs.close();
}

void TorchModelManager::estimation()
{
    auto inputs = torch::randn({ 1, NET_INPUT_SIZE });
    auto inputTensorAccessor = inputs.accessor<float, 2>();
    for (int i = 0; i < NET_INPUT_SIZE; i++)
    {
        inputTensorAccessor.data()[i] = sharedMemory->NETInputs[i];
    }
    sharedMemory->estimatedGRF = mANN->forward(inputs).item<float>();
}

void TorchModelManager::onlineLearning()
{

}