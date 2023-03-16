//
// Created by jaehoon on 23. 3. 15.
//

#include <torch/torch.h>
#include <canine-leg_simulation/SimulMain.hpp>
#include <SimulationGUI/mainwindow.h>
#include <QApplication>

extern pSHM sharedMemory;

pthread_t NRTThreadTorch;

void torchFunction()
{
    while (true)
    {
        switch (sharedMemory->torchState)
        {
            case TORCH_NO_ACT:
            {
                break;
            }
            case TORCH_LOAD_MODEL:
            {
                torch::Tensor tensor = torch::rand({ 2, 3 });
                std::cout << tensor << std::endl;
                std::cout << *(sharedMemory->modelName) << std::endl;
                sharedMemory->torchState = TORCH_NO_ACT;
                break;
            }
            case TORCH_ONLINE_LEARNING:
            {
                break;
            }
            default:
                break;
        }
        usleep(1000);
    }
}

void* NRTTorchThread(void* arg)
{
    std::cout << "entered #nrt_torch_thread" << std::endl;
    while (true)
    {
        torchFunction();
        usleep(CMD_dT * 1e6);
    }
}


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    StartSimulation();
    int thread_id_nrt2 = generate_nrt_thread(NRTThreadTorch, NRTTorchThread, "nrt_thread2", 2, NULL);
    w.show();
    return a.exec();
}

