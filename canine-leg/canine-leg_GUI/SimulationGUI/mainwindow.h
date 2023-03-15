#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTimer>
#include <QPen>
#include <canine-leg_util/SharedMemory.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_BT_MAIN_PANEL_START_SIMULATION_clicked();
    void on_BT_MAIN_PANEL_RESET_clicked();
    void on_BT_MAIN_PANEL_HOME_clicked();
    void on_BT_PARAMETERS_SET_PARAMS_clicked();
    void on_BT_CUBIC_CONTROL_clicked();
    void on_BT_COS_CONTROL_clicked();
    void on_BT_MODEL_LOAD_clicked();
    void on_BT_MODEL_ONLINE_LEARNING_clicked();
    void on_BT_EXPERIMENT_DATA_SAVE_clicked();

private:
    void displayUpdate();
    void graphUpdate();
    void graphInitialize();
    void initTable(QTableWidget *table);
    void initLineEdit();

private:
    QTimer      *displayTimer;
    QTimer		*graphTimer;
    Ui::MainWindow *ui;
    double graphOffset;
};
#endif // MAINWINDOW_H
