//
// Created by jaehoon on 23. 3. 17.
//

#include "TorchModelManager.hpp"

extern pSHM sharedMemory;

TorchModelManager::TorchModelManager()
{
    mFCN_GRF = std::make_shared<GRFNet>(GRF_NET_INPUT_SIZE, GRF_NET_OUTPUT_SIZE);
    mGRFNetInputs = torch::randn({ 1, GRF_NET_INPUT_SIZE });
    mGRFNetOutputs = torch::randn({ 1, GRF_NET_OUTPUT_SIZE });
    mGRFNetTargets = torch::randn({ 1, GRF_NET_OUTPUT_SIZE });
    mFCN_Sim2Realnet = std::make_shared<Sim2RealNet>(SIM2REAL_NET_INPUT_SIZE, SIM2REAL_NET_OUTPUT_SIZE);
    mSim2RealNetInputs = torch::randn({ 1, SIM2REAL_NET_INPUT_SIZE });
    mSim2RealNetOutputs = torch::randn({ 1, SIM2REAL_NET_OUTPUT_SIZE });
    mSim2RealNetTargets = torch::randn({ 1, SIM2REAL_NET_OUTPUT_SIZE });
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
        sharedMemory->torchState = TORCH_ESTIMATION;
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
    std::string modelPathGRF;
    modelPathGRF = *(sharedMemory->modelName);
    std::ifstream ifs(modelPathGRF);
    try
    {
        if (!ifs.is_open())
        {
            throw modelPathGRF;
        }
        torch::load(mFCN_GRF, modelPathGRF);
        std::cout << "[MODEL MANAGER] : " << modelPathGRF << " is loaded." << std::endl << std::endl;
    }
    catch (std::string path)
    {
        perror("[MODEL MANAGER] Model is not loaded.");
        std::cout << "[MODEL MANAGER] Given path : " << path << std::endl << std::endl;
    }
    ifs.close();

    std::string modelPathSim2Real;
    modelPathSim2Real.append(MODEL_DIR);
    modelPathSim2Real.append("Model_Sim2Real_5_cos_1.pt");
    std::ifstream ifs2(modelPathSim2Real);
    try
    {
        if (!ifs2.is_open())
        {
            throw modelPathSim2Real;
        }
        torch::load(mFCN_Sim2Realnet, modelPathSim2Real);
        std::cout << "[MODEL MANAGER] : " << modelPathSim2Real << " is loaded." << std::endl << std::endl;
    }
    catch (std::string path)
    {
        perror("[MODEL MANAGER] Model is not loaded.");
        std::cout << "[MODEL MANAGER] Given path : " << path << std::endl << std::endl;
    }
    ifs2.close();
}

void TorchModelManager::estimation()
{
    auto inputGRFNetTensorAccessor = mGRFNetInputs.accessor<float, 2>();
    auto inputSim2RealNetTensorAccessor = mSim2RealNetInputs.accessor<float, 2>();

    for (int i = 0; i < GRF_NET_INPUT_SIZE; i++)
    {
        inputGRFNetTensorAccessor.data()[i] = sharedMemory->GRFNETInputs[i];
    }
    sharedMemory->estimatedGRF = mFCN_GRF->forward(mGRFNetInputs).item<float>();

    for (int i = 0; i < SIM2REAL_NET_INPUT_SIZE; i++)
    {
        inputSim2RealNetTensorAccessor.data()[i] = sharedMemory->Sim2RealNETInputs[i];
    }
    sharedMemory->sim2realGRF = mFCN_Sim2Realnet->forward(mSim2RealNetInputs).item<float>();
//    std::cout << "[MODEL MANAGER] estimated GRF : " << sharedMemory->estimatedGRF << std::endl;
}

void TorchModelManager::onlineLearning()
{
//    torch::optim::SGD optimizer(mFCN_GRF->parameters(), torch::optim::SGDOptions(0.001));
////    torch::optim::Adam optimizer(mANN->parameters(), torch::optim::AdamOptions(sharedMemory->learningRate));
//    auto inputTensorAccessor = mInputs.accessor<float, 2>();
//    auto targetTensorAccessor = mTargets.accessor<float, 2>();
//
//    optimizer.zero_grad();
//    for (int i = 0; i < NET_INPUT_SIZE; i++)
//    {
//        inputTensorAccessor.data()[i] = sharedMemory->NETInputs[i];
//    }
//    targetTensorAccessor.data()[0] = sharedMemory->measuredGRF;
//    auto out = mFCN_GRF->forward(mInputs);
//    auto loss = torch::mse_loss(out, mTargets);
//    loss.backward();
//    optimizer.step();
//    std::cout << "[MODEL MANAGER] online-learning loss : " << loss.item<float>() << std::endl;
//    sharedMemory->estimatedGRF = out.item<float>();
}