//
// Created by jaehoon on 23. 3. 15.
//

#include <SimulationGUI/mainwindow.h>
#include <canine-leg_simulation/SimulMain.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    StartSimulation();
    w.show();
    return a.exec();
}

