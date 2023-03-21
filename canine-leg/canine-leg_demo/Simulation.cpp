//
// Created by jaehoon on 23. 3. 15.
//

#include "TorchModelManager.hpp"
#include <canine-leg_simulation/SimulMain.hpp>
#include <SimulationGUI/mainwindow.h>
#include <QApplication>


pthread_t NRTThreadTorch;

TorchModelManager torchManager;

void* NRTTorchThread(void* arg)
{
    std::cout << "entered #nrt_torch_thread" << std::endl;
    while (true)
    {
        torchManager.torchFunction();
        usleep(TORCH_dT * 1e6);
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

