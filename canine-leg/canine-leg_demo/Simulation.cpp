//
// Created by jaehoon on 23. 3. 15.
//

#include <SimulationGUI/mainwindow.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

