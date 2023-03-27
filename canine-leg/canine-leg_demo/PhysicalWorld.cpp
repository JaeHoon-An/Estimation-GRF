//
// Created by jaehoon on 23. 3. 27.
//


#include "TorchModelManager.hpp"
#include <canine-leg_real/PhysicalWorldMain.hpp>
#include <ControlGUI/mainwindow.h>
#include <QApplication>


pthread_t NRTThreadTorch;

TorchModelManager torchManager;

void* NRTTorchThread(void* arg)
{
    std::cout << "entered #nrt_torch_thread" << std::endl;
    struct timespec TIME_1;
    struct timespec TIME_2;

    while (true)
    {
        clock_gettime(CLOCK_REALTIME, &TIME_1);
        torchManager.torchFunction();
        clock_gettime(CLOCK_REALTIME, &TIME_2);

        /**
         * It takes about 70 ~ 311 micro seconds. / mean : 120 micro seconds
         */
//        std::cout << "one estimation time : "<<timediff_us(&TIME_1, &TIME_2) << std::endl;
        usleep(TORCH_dT * 1e6);
    }
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    StartFSM();
    int thread_id_nrt2 = generate_nrt_thread(NRTThreadTorch, NRTTorchThread, "nrt_thread2", 2, NULL);
    w.show();
    return a.exec();
}

