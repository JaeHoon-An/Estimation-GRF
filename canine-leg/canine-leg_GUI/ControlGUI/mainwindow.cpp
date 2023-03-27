#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_BT_MAIN_PANEL_CAN_ON_clicked()
{

}

void MainWindow::on_BT_MAIN_PANEL_MOTOR_ON_clicked()
{

}

void MainWindow::on_BT_MAIN_PANEL_MOTOR_OFF_clicked()
{

}

void MainWindow::on_BT_MAIN_PANEL_HOME_clicked()
{

}

void MainWindow::on_BT_PARAMETERS_SET_PARAMS_clicked()
{

}

void MainWindow::on_BT_CUBIC_CONTROL_clicked()
{

}

void MainWindow::on_BT_COS_CONTROL_clicked()
{

}

void MainWindow::on_BT_MODEL_LOAD_clicked()
{

}

void MainWindow::on_BT_MODEL_ONLINE_LEARNING_clicked()
{

}

void MainWindow::on_BT_EXPERIMENT_DATA_SAVE_clicked()
{

}

void MainWindow::on_BT_CUSTOM_BUTTON_CUSTOM_1_clicked()
{

}

void MainWindow::on_BT_CUSTOM_BUTTON_CUSTOM_2_clicked()
{

}

void MainWindow::on_BT_CUSTOM_BUTTON_CUSTOM_3_clicked()
{

}

void MainWindow::on_BT_CUSTOM_BUTTON_CUSTOM_4_clicked()
{

}
