//
// Created by jaehoon on 23. 3. 17.
//

#ifndef ESTIMATION_GRF_TORCHMODELMANAGER_HPP
#define ESTIMATION_GRF_TORCHMODELMANAGER_HPP

#include <torch/torch.h>
#include <canine-leg_util/SharedMemory.hpp>

class TorchModelManager
{
public:
    TorchModelManager();
    void torchFunction();

private:
    struct swish : torch::nn::Module
    {
        torch::Tensor forward(torch::Tensor x)
        {
            x = x * torch::sigmoid(x);
            return x;
        }
    };

    struct Net : torch::nn::Module
    {
        torch::nn::Linear fc1{ nullptr }, fc2{ nullptr }, fc3{ nullptr }, fc4{ nullptr }, fc5{ nullptr }, fc6{ nullptr }, fc7{ nullptr }, fc8{ nullptr }, fc9{ nullptr };
        swish activationSwish;

        Net(int in_dim, int out_dim)
        {
            fc1 = register_module("fc1", torch::nn::Linear(in_dim, 32));
            fc2 = register_module("fc2", torch::nn::Linear(32, 32));
            fc3 = register_module("fc3", torch::nn::Linear(32, 32));
            fc4 = register_module("fc4", torch::nn::Linear(32, 32));
            fc5 = register_module("fc5", torch::nn::Linear(32, 32));
            fc6 = register_module("fc6", torch::nn::Linear(32, 32));
            fc7 = register_module("fc7", torch::nn::Linear(32, 32));
            fc8 = register_module("fc8", torch::nn::Linear(32, 32));
            fc9 = register_module("fc9", torch::nn::Linear(32, out_dim));
        }

        torch::Tensor forward(torch::Tensor x)
        {
            x = fc1->forward(x);
            x = fc2->forward(activationSwish.forward(x));
            x = fc3->forward(activationSwish.forward(x));
            x = fc4->forward(activationSwish.forward(x));
            x = fc5->forward(activationSwish.forward(x));
            x = fc6->forward(activationSwish.forward(x));
            x = fc7->forward(activationSwish.forward(x));
            x = fc8->forward(activationSwish.forward(x));
            x = fc9->forward(activationSwish.forward(x));

            return x;
        }
    };

private:
    void loadModel();
    void estimation();
    void onlineLearning();

private:
    std::shared_ptr<Net> mANN;
};


#endif //ESTIMATION_GRF_TORCHMODELMANAGER_HPP
