/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include "../util/qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *GB_MODEL_MANAGEMENT;
    QLineEdit *LE_MODEL_LOAD_PATH;
    QPushButton *BT_MODEL_LOAD;
    QLineEdit *LE_MODEL_LEARNING_RATE;
    QPushButton *BT_MODEL_ONLINE_LEARNING;
    QLineEdit *LE_MODEL_PARAMS_LEARNING_RATE;
    QTabWidget *tabWidget;
    QWidget *TW_STATES;
    QCustomPlot *QCP_STATES_POS_HIP;
    QCustomPlot *QCP_STATES_POS_KNEE;
    QCustomPlot *QCP_STATES_VEL_HIP;
    QCustomPlot *QCP_STATES_VEL_KNEE;
    QCustomPlot *QCP_STATES_TAU_HIP;
    QCustomPlot *QCP_STATES_TAU_KNEE;
    QWidget *TW_GRF;
    QCustomPlot *QCP_GRF_GRF;
    QCustomPlot *QCP_GRF_ERROR;
    QGroupBox *GB_EXPERIMENT_DATA_MANAGEMENT;
    QLineEdit *LE_EXPERIMENT_DATA_PATH;
    QPushButton *BT_EXPERIMENT_DATA_SAVE;
    QGroupBox *GB_COS;
    QLineEdit *LE_COS_PARAMS_AMPLITUDE;
    QLineEdit *LE_COS_PARAMS_FREQUENCY;
    QLineEdit *LE_COS_AMPLITUDE;
    QLineEdit *LE_COS_FREQUENCY;
    QPushButton *BT_COS_CONTROL;
    QGroupBox *GB_CUBIC;
    QLineEdit *LE_CUBIC_PARAMS_GOAL_HEIGHT;
    QLineEdit *LE_CUBIC_PARAMS_TIME_DURATION;
    QLineEdit *LE_CUBIC_GOAL_HEIGHT;
    QLineEdit *LE_CUBIC_TIME_DURATION;
    QPushButton *BT_CUBIC_CONTROL;
    QGroupBox *GB_PARAMETERS;
    QLineEdit *LE_PARAMETERS_PARAMS_Kp;
    QLineEdit *LE_PARAMETERS_PARAMS_Kd;
    QLineEdit *LE_PARAMETERS_Kp;
    QLineEdit *LE_PARAMETERS_Kd;
    QPushButton *BT_PARAMETERS_SET_PARAMS;
    QLineEdit *LE_PARAMETERS_PARAMS_LPF_VEL;
    QLineEdit *LE_PARAMETERS_LPF_VEL;
    QGroupBox *GB_MAIN_PANEL;
    QPushButton *BT_MAIN_PANEL_RESET;
    QPushButton *BT_MAIN_PANEL_START_SIMULATION;
    QPushButton *BT_MAIN_PANEL_HOME;
    QTableWidget *tableWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1420, 997);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        GB_MODEL_MANAGEMENT = new QGroupBox(centralwidget);
        GB_MODEL_MANAGEMENT->setObjectName(QString::fromUtf8("GB_MODEL_MANAGEMENT"));
        GB_MODEL_MANAGEMENT->setGeometry(QRect(10, 690, 381, 110));
        LE_MODEL_LOAD_PATH = new QLineEdit(GB_MODEL_MANAGEMENT);
        LE_MODEL_LOAD_PATH->setObjectName(QString::fromUtf8("LE_MODEL_LOAD_PATH"));
        LE_MODEL_LOAD_PATH->setGeometry(QRect(10, 30, 260, 25));
        BT_MODEL_LOAD = new QPushButton(GB_MODEL_MANAGEMENT);
        BT_MODEL_LOAD->setObjectName(QString::fromUtf8("BT_MODEL_LOAD"));
        BT_MODEL_LOAD->setGeometry(QRect(280, 30, 90, 25));
        LE_MODEL_LEARNING_RATE = new QLineEdit(GB_MODEL_MANAGEMENT);
        LE_MODEL_LEARNING_RATE->setObjectName(QString::fromUtf8("LE_MODEL_LEARNING_RATE"));
        LE_MODEL_LEARNING_RATE->setEnabled(false);
        LE_MODEL_LEARNING_RATE->setGeometry(QRect(10, 65, 130, 30));
        LE_MODEL_LEARNING_RATE->setAlignment(Qt::AlignCenter);
        BT_MODEL_ONLINE_LEARNING = new QPushButton(GB_MODEL_MANAGEMENT);
        BT_MODEL_ONLINE_LEARNING->setObjectName(QString::fromUtf8("BT_MODEL_ONLINE_LEARNING"));
        BT_MODEL_ONLINE_LEARNING->setGeometry(QRect(280, 60, 90, 40));
        LE_MODEL_PARAMS_LEARNING_RATE = new QLineEdit(GB_MODEL_MANAGEMENT);
        LE_MODEL_PARAMS_LEARNING_RATE->setObjectName(QString::fromUtf8("LE_MODEL_PARAMS_LEARNING_RATE"));
        LE_MODEL_PARAMS_LEARNING_RATE->setGeometry(QRect(140, 65, 130, 30));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(400, 10, 1010, 940));
        TW_STATES = new QWidget();
        TW_STATES->setObjectName(QString::fromUtf8("TW_STATES"));
        QCP_STATES_POS_HIP = new QCustomPlot(TW_STATES);
        QCP_STATES_POS_HIP->setObjectName(QString::fromUtf8("QCP_STATES_POS_HIP"));
        QCP_STATES_POS_HIP->setGeometry(QRect(10, 10, 490, 290));
        QCP_STATES_POS_KNEE = new QCustomPlot(TW_STATES);
        QCP_STATES_POS_KNEE->setObjectName(QString::fromUtf8("QCP_STATES_POS_KNEE"));
        QCP_STATES_POS_KNEE->setGeometry(QRect(510, 10, 490, 290));
        QCP_STATES_VEL_HIP = new QCustomPlot(TW_STATES);
        QCP_STATES_VEL_HIP->setObjectName(QString::fromUtf8("QCP_STATES_VEL_HIP"));
        QCP_STATES_VEL_HIP->setGeometry(QRect(10, 310, 490, 290));
        QCP_STATES_VEL_KNEE = new QCustomPlot(TW_STATES);
        QCP_STATES_VEL_KNEE->setObjectName(QString::fromUtf8("QCP_STATES_VEL_KNEE"));
        QCP_STATES_VEL_KNEE->setGeometry(QRect(510, 310, 490, 290));
        QCP_STATES_TAU_HIP = new QCustomPlot(TW_STATES);
        QCP_STATES_TAU_HIP->setObjectName(QString::fromUtf8("QCP_STATES_TAU_HIP"));
        QCP_STATES_TAU_HIP->setGeometry(QRect(10, 610, 490, 290));
        QCP_STATES_TAU_KNEE = new QCustomPlot(TW_STATES);
        QCP_STATES_TAU_KNEE->setObjectName(QString::fromUtf8("QCP_STATES_TAU_KNEE"));
        QCP_STATES_TAU_KNEE->setGeometry(QRect(510, 610, 490, 290));
        tabWidget->addTab(TW_STATES, QString());
        TW_GRF = new QWidget();
        TW_GRF->setObjectName(QString::fromUtf8("TW_GRF"));
        QCP_GRF_GRF = new QCustomPlot(TW_GRF);
        QCP_GRF_GRF->setObjectName(QString::fromUtf8("QCP_GRF_GRF"));
        QCP_GRF_GRF->setGeometry(QRect(10, 10, 990, 440));
        QCP_GRF_ERROR = new QCustomPlot(TW_GRF);
        QCP_GRF_ERROR->setObjectName(QString::fromUtf8("QCP_GRF_ERROR"));
        QCP_GRF_ERROR->setGeometry(QRect(10, 460, 990, 440));
        tabWidget->addTab(TW_GRF, QString());
        GB_EXPERIMENT_DATA_MANAGEMENT = new QGroupBox(centralwidget);
        GB_EXPERIMENT_DATA_MANAGEMENT->setObjectName(QString::fromUtf8("GB_EXPERIMENT_DATA_MANAGEMENT"));
        GB_EXPERIMENT_DATA_MANAGEMENT->setGeometry(QRect(10, 810, 381, 62));
        LE_EXPERIMENT_DATA_PATH = new QLineEdit(GB_EXPERIMENT_DATA_MANAGEMENT);
        LE_EXPERIMENT_DATA_PATH->setObjectName(QString::fromUtf8("LE_EXPERIMENT_DATA_PATH"));
        LE_EXPERIMENT_DATA_PATH->setGeometry(QRect(10, 29, 260, 25));
        BT_EXPERIMENT_DATA_SAVE = new QPushButton(GB_EXPERIMENT_DATA_MANAGEMENT);
        BT_EXPERIMENT_DATA_SAVE->setObjectName(QString::fromUtf8("BT_EXPERIMENT_DATA_SAVE"));
        BT_EXPERIMENT_DATA_SAVE->setGeometry(QRect(280, 29, 90, 25));
        GB_COS = new QGroupBox(centralwidget);
        GB_COS->setObjectName(QString::fromUtf8("GB_COS"));
        GB_COS->setGeometry(QRect(10, 580, 381, 100));
        LE_COS_PARAMS_AMPLITUDE = new QLineEdit(GB_COS);
        LE_COS_PARAMS_AMPLITUDE->setObjectName(QString::fromUtf8("LE_COS_PARAMS_AMPLITUDE"));
        LE_COS_PARAMS_AMPLITUDE->setGeometry(QRect(129, 33, 141, 25));
        LE_COS_PARAMS_FREQUENCY = new QLineEdit(GB_COS);
        LE_COS_PARAMS_FREQUENCY->setObjectName(QString::fromUtf8("LE_COS_PARAMS_FREQUENCY"));
        LE_COS_PARAMS_FREQUENCY->setGeometry(QRect(130, 63, 140, 25));
        LE_COS_AMPLITUDE = new QLineEdit(GB_COS);
        LE_COS_AMPLITUDE->setObjectName(QString::fromUtf8("LE_COS_AMPLITUDE"));
        LE_COS_AMPLITUDE->setEnabled(false);
        LE_COS_AMPLITUDE->setGeometry(QRect(10, 33, 111, 25));
        LE_COS_AMPLITUDE->setLayoutDirection(Qt::LeftToRight);
        LE_COS_AMPLITUDE->setAlignment(Qt::AlignCenter);
        LE_COS_FREQUENCY = new QLineEdit(GB_COS);
        LE_COS_FREQUENCY->setObjectName(QString::fromUtf8("LE_COS_FREQUENCY"));
        LE_COS_FREQUENCY->setEnabled(false);
        LE_COS_FREQUENCY->setGeometry(QRect(10, 63, 111, 25));
        LE_COS_FREQUENCY->setLayoutDirection(Qt::LeftToRight);
        LE_COS_FREQUENCY->setAlignment(Qt::AlignCenter);
        BT_COS_CONTROL = new QPushButton(GB_COS);
        BT_COS_CONTROL->setObjectName(QString::fromUtf8("BT_COS_CONTROL"));
        BT_COS_CONTROL->setGeometry(QRect(280, 33, 89, 55));
        GB_CUBIC = new QGroupBox(centralwidget);
        GB_CUBIC->setObjectName(QString::fromUtf8("GB_CUBIC"));
        GB_CUBIC->setGeometry(QRect(10, 470, 381, 100));
        LE_CUBIC_PARAMS_GOAL_HEIGHT = new QLineEdit(GB_CUBIC);
        LE_CUBIC_PARAMS_GOAL_HEIGHT->setObjectName(QString::fromUtf8("LE_CUBIC_PARAMS_GOAL_HEIGHT"));
        LE_CUBIC_PARAMS_GOAL_HEIGHT->setGeometry(QRect(129, 33, 141, 25));
        LE_CUBIC_PARAMS_TIME_DURATION = new QLineEdit(GB_CUBIC);
        LE_CUBIC_PARAMS_TIME_DURATION->setObjectName(QString::fromUtf8("LE_CUBIC_PARAMS_TIME_DURATION"));
        LE_CUBIC_PARAMS_TIME_DURATION->setGeometry(QRect(130, 63, 140, 25));
        LE_CUBIC_GOAL_HEIGHT = new QLineEdit(GB_CUBIC);
        LE_CUBIC_GOAL_HEIGHT->setObjectName(QString::fromUtf8("LE_CUBIC_GOAL_HEIGHT"));
        LE_CUBIC_GOAL_HEIGHT->setEnabled(false);
        LE_CUBIC_GOAL_HEIGHT->setGeometry(QRect(10, 33, 111, 25));
        LE_CUBIC_GOAL_HEIGHT->setLayoutDirection(Qt::LeftToRight);
        LE_CUBIC_GOAL_HEIGHT->setAlignment(Qt::AlignCenter);
        LE_CUBIC_TIME_DURATION = new QLineEdit(GB_CUBIC);
        LE_CUBIC_TIME_DURATION->setObjectName(QString::fromUtf8("LE_CUBIC_TIME_DURATION"));
        LE_CUBIC_TIME_DURATION->setEnabled(false);
        LE_CUBIC_TIME_DURATION->setGeometry(QRect(10, 63, 111, 25));
        LE_CUBIC_TIME_DURATION->setLayoutDirection(Qt::LeftToRight);
        LE_CUBIC_TIME_DURATION->setAlignment(Qt::AlignCenter);
        BT_CUBIC_CONTROL = new QPushButton(GB_CUBIC);
        BT_CUBIC_CONTROL->setObjectName(QString::fromUtf8("BT_CUBIC_CONTROL"));
        BT_CUBIC_CONTROL->setGeometry(QRect(280, 33, 89, 55));
        GB_PARAMETERS = new QGroupBox(centralwidget);
        GB_PARAMETERS->setObjectName(QString::fromUtf8("GB_PARAMETERS"));
        GB_PARAMETERS->setGeometry(QRect(10, 330, 381, 130));
        LE_PARAMETERS_PARAMS_Kp = new QLineEdit(GB_PARAMETERS);
        LE_PARAMETERS_PARAMS_Kp->setObjectName(QString::fromUtf8("LE_PARAMETERS_PARAMS_Kp"));
        LE_PARAMETERS_PARAMS_Kp->setGeometry(QRect(129, 33, 141, 25));
        LE_PARAMETERS_PARAMS_Kd = new QLineEdit(GB_PARAMETERS);
        LE_PARAMETERS_PARAMS_Kd->setObjectName(QString::fromUtf8("LE_PARAMETERS_PARAMS_Kd"));
        LE_PARAMETERS_PARAMS_Kd->setGeometry(QRect(130, 63, 140, 25));
        LE_PARAMETERS_Kp = new QLineEdit(GB_PARAMETERS);
        LE_PARAMETERS_Kp->setObjectName(QString::fromUtf8("LE_PARAMETERS_Kp"));
        LE_PARAMETERS_Kp->setEnabled(false);
        LE_PARAMETERS_Kp->setGeometry(QRect(10, 33, 111, 25));
        LE_PARAMETERS_Kp->setLayoutDirection(Qt::LeftToRight);
        LE_PARAMETERS_Kp->setAlignment(Qt::AlignCenter);
        LE_PARAMETERS_Kd = new QLineEdit(GB_PARAMETERS);
        LE_PARAMETERS_Kd->setObjectName(QString::fromUtf8("LE_PARAMETERS_Kd"));
        LE_PARAMETERS_Kd->setEnabled(false);
        LE_PARAMETERS_Kd->setGeometry(QRect(10, 63, 111, 25));
        LE_PARAMETERS_Kd->setLayoutDirection(Qt::LeftToRight);
        LE_PARAMETERS_Kd->setAlignment(Qt::AlignCenter);
        BT_PARAMETERS_SET_PARAMS = new QPushButton(GB_PARAMETERS);
        BT_PARAMETERS_SET_PARAMS->setObjectName(QString::fromUtf8("BT_PARAMETERS_SET_PARAMS"));
        BT_PARAMETERS_SET_PARAMS->setGeometry(QRect(280, 33, 89, 83));
        LE_PARAMETERS_PARAMS_LPF_VEL = new QLineEdit(GB_PARAMETERS);
        LE_PARAMETERS_PARAMS_LPF_VEL->setObjectName(QString::fromUtf8("LE_PARAMETERS_PARAMS_LPF_VEL"));
        LE_PARAMETERS_PARAMS_LPF_VEL->setGeometry(QRect(130, 92, 140, 25));
        LE_PARAMETERS_LPF_VEL = new QLineEdit(GB_PARAMETERS);
        LE_PARAMETERS_LPF_VEL->setObjectName(QString::fromUtf8("LE_PARAMETERS_LPF_VEL"));
        LE_PARAMETERS_LPF_VEL->setEnabled(false);
        LE_PARAMETERS_LPF_VEL->setGeometry(QRect(10, 92, 111, 25));
        LE_PARAMETERS_LPF_VEL->setLayoutDirection(Qt::LeftToRight);
        LE_PARAMETERS_LPF_VEL->setAlignment(Qt::AlignCenter);
        GB_MAIN_PANEL = new QGroupBox(centralwidget);
        GB_MAIN_PANEL->setObjectName(QString::fromUtf8("GB_MAIN_PANEL"));
        GB_MAIN_PANEL->setGeometry(QRect(15, 20, 370, 100));
        BT_MAIN_PANEL_RESET = new QPushButton(GB_MAIN_PANEL);
        BT_MAIN_PANEL_RESET->setObjectName(QString::fromUtf8("BT_MAIN_PANEL_RESET"));
        BT_MAIN_PANEL_RESET->setGeometry(QRect(130, 30, 110, 60));
        BT_MAIN_PANEL_START_SIMULATION = new QPushButton(GB_MAIN_PANEL);
        BT_MAIN_PANEL_START_SIMULATION->setObjectName(QString::fromUtf8("BT_MAIN_PANEL_START_SIMULATION"));
        BT_MAIN_PANEL_START_SIMULATION->setGeometry(QRect(10, 30, 110, 60));
        BT_MAIN_PANEL_HOME = new QPushButton(GB_MAIN_PANEL);
        BT_MAIN_PANEL_HOME->setObjectName(QString::fromUtf8("BT_MAIN_PANEL_HOME"));
        BT_MAIN_PANEL_HOME->setGeometry(QRect(250, 30, 110, 60));
        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(10, 130, 380, 192));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1420, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        GB_MODEL_MANAGEMENT->setTitle(QApplication::translate("MainWindow", "Model management", nullptr));
        LE_MODEL_LOAD_PATH->setText(QString());
        BT_MODEL_LOAD->setText(QApplication::translate("MainWindow", "LOAD", nullptr));
        LE_MODEL_LEARNING_RATE->setText(QApplication::translate("MainWindow", "Learning rate :", nullptr));
        BT_MODEL_ONLINE_LEARNING->setText(QApplication::translate("MainWindow", "ONLINE\n"
                                                                                "LEARNING", nullptr));
        LE_MODEL_PARAMS_LEARNING_RATE->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(TW_STATES), QApplication::translate("MainWindow", "States", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(TW_GRF), QApplication::translate("MainWindow", "GRF", nullptr));
        GB_EXPERIMENT_DATA_MANAGEMENT->setTitle(QApplication::translate("MainWindow", "Experiment data management", nullptr));
        LE_EXPERIMENT_DATA_PATH->setText(QString());
        BT_EXPERIMENT_DATA_SAVE->setText(QApplication::translate("MainWindow", "SAVE", nullptr));
        GB_COS->setTitle(QApplication::translate("MainWindow", "COS motion controller", nullptr));
        LE_COS_PARAMS_AMPLITUDE->setText(QString());
        LE_COS_PARAMS_FREQUENCY->setText(QString());
        LE_COS_AMPLITUDE->setText(QApplication::translate("MainWindow", "Amplitude", nullptr));
        LE_COS_FREQUENCY->setText(QApplication::translate("MainWindow", "Frequency", nullptr));
        BT_COS_CONTROL->setText(QApplication::translate("MainWindow", "CONTROL", nullptr));
        GB_CUBIC->setTitle(QApplication::translate("MainWindow", "Cubic motion controller", nullptr));
        LE_CUBIC_PARAMS_GOAL_HEIGHT->setText(QString());
        LE_CUBIC_PARAMS_TIME_DURATION->setText(QString());
        LE_CUBIC_GOAL_HEIGHT->setText(QApplication::translate("MainWindow", "Goal height", nullptr));
        LE_CUBIC_TIME_DURATION->setText(QApplication::translate("MainWindow", "Time duration", nullptr));
        BT_CUBIC_CONTROL->setText(QApplication::translate("MainWindow", "CONTROL", nullptr));
        GB_PARAMETERS->setTitle(QApplication::translate("MainWindow", "Parameters", nullptr));
        LE_PARAMETERS_PARAMS_Kp->setText(QString());
        LE_PARAMETERS_PARAMS_Kd->setText(QString());
        LE_PARAMETERS_Kp->setText(QApplication::translate("MainWindow", "Kp", nullptr));
        LE_PARAMETERS_Kd->setText(QApplication::translate("MainWindow", "Kd", nullptr));
        BT_PARAMETERS_SET_PARAMS->setText(QApplication::translate("MainWindow", "SET\n"
                                                                                "PARAMS", nullptr));
        LE_PARAMETERS_PARAMS_LPF_VEL->setText(QString());
        LE_PARAMETERS_LPF_VEL->setText(QApplication::translate("MainWindow", "LPF Velocity", nullptr));
        GB_MAIN_PANEL->setTitle(QString());
        BT_MAIN_PANEL_RESET->setText(QApplication::translate("MainWindow", "RESET", nullptr));
        BT_MAIN_PANEL_START_SIMULATION->setText(QApplication::translate("MainWindow", "START\n"
                                                                                      "SIMULATION", nullptr));
        BT_MAIN_PANEL_HOME->setText(QApplication::translate("MainWindow", "HOME", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
