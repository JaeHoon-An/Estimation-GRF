#include "mainwindow.h"
#include "ui_mainwindow.h"

extern pUI_COMMAND sharedCommand;
extern pSHM sharedMemory;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initLineEdit();
    initTable(ui->tableWidget);
    graphInitialize();

    displayTimer = new QTimer();
    connect(displayTimer, SIGNAL(timeout()), this, SLOT(displayUpdate()));
    displayTimer->start(20);
    graphTimer = new QTimer();
    connect(graphTimer, SIGNAL(timeout()), this, SLOT(graphUpdate()));
    graphTimer->start(20);
    graphOffset = 10.0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//TODO
void MainWindow::initLineEdit()
{

}

void MainWindow::graphInitialize()
{
    QPen myPen, dotPen, dashPen;
    myPen.setWidthF(1.5);
    myPen.setColor(Qt::blue);
    dotPen.setStyle(Qt::DotLine);
    dotPen.setWidth(1);
    dotPen.setColor(QColor(180,180,180));
    dashPen.setStyle(Qt::DashLine);
    dashPen.setWidth(2);
    dashPen.setColor(Qt::gray);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");

    ui->QCP_GRF_ERROR->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP :: iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->QCP_GRF_GRF->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP :: iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->QCP_STATES_POS_HIP->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP :: iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->QCP_STATES_POS_KNEE->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP :: iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->QCP_STATES_VEL_HIP->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP :: iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->QCP_STATES_VEL_KNEE->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP :: iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->QCP_STATES_TAU_HIP->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP :: iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->QCP_STATES_TAU_KNEE->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP :: iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);

    ui->QCP_GRF_ERROR->legend->setVisible(true);
    ui->QCP_GRF_ERROR->legend->setFont(QFont("Helvetica", 9));
    ui->QCP_GRF_ERROR->addGraph();
    ui->QCP_GRF_ERROR->graph(0)->setPen(dotPen);
    ui->QCP_GRF_ERROR->graph(0)->setName("Absolute Error");

    ui->QCP_GRF_GRF->legend->setVisible(true);
    ui->QCP_GRF_GRF->legend->setFont(QFont("Helvetica", 9));
    ui->QCP_GRF_GRF->addGraph();
    ui->QCP_GRF_GRF->graph(0)->setPen(dashPen);
    ui->QCP_GRF_GRF->graph(0)->setName("Measured GRF");
    ui->QCP_GRF_GRF->addGraph();
    ui->QCP_GRF_GRF->graph(1)->setPen(myPen);
    ui->QCP_GRF_GRF->graph(1)->setName("Estimated GRF");

    ui->QCP_STATES_POS_HIP->legend->setVisible(true);
    ui->QCP_STATES_POS_HIP->legend->setFont(QFont("Helvetica", 9));
    ui->QCP_STATES_POS_HIP->addGraph();
    ui->QCP_STATES_POS_HIP->graph(0)->setPen(dashPen);
    ui->QCP_STATES_POS_HIP->graph(0)->setName("desired hip pos");
    ui->QCP_STATES_POS_HIP->addGraph();
    ui->QCP_STATES_POS_HIP->graph(1)->setPen(myPen);
    ui->QCP_STATES_POS_HIP->graph(1)->setName("current hip pos");

    ui->QCP_STATES_POS_KNEE->legend->setVisible(true);
    ui->QCP_STATES_POS_KNEE->legend->setFont(QFont("Helvetica", 9));
    ui->QCP_STATES_POS_KNEE->addGraph();
    ui->QCP_STATES_POS_KNEE->graph(0)->setPen(dashPen);
    ui->QCP_STATES_POS_KNEE->graph(0)->setName("desired knee pos");
    ui->QCP_STATES_POS_KNEE->addGraph();
    ui->QCP_STATES_POS_KNEE->graph(1)->setPen(myPen);
    ui->QCP_STATES_POS_KNEE->graph(1)->setName("current knee pos");

    ui->QCP_STATES_VEL_HIP->legend->setVisible(true);
    ui->QCP_STATES_VEL_HIP->legend->setFont(QFont("Helvetica", 9));
    ui->QCP_STATES_VEL_HIP->addGraph();
    ui->QCP_STATES_VEL_HIP->graph(0)->setPen(dashPen);
    ui->QCP_STATES_VEL_HIP->graph(0)->setName("desired hip vel");
    ui->QCP_STATES_VEL_HIP->addGraph();
    ui->QCP_STATES_VEL_HIP->graph(1)->setPen(myPen);
    ui->QCP_STATES_VEL_HIP->graph(1)->setName("current hip vel");

    ui->QCP_STATES_VEL_KNEE->legend->setVisible(true);
    ui->QCP_STATES_VEL_KNEE->legend->setFont(QFont("Helvetica", 9));
    ui->QCP_STATES_VEL_KNEE->addGraph();
    ui->QCP_STATES_VEL_KNEE->graph(0)->setPen(dashPen);
    ui->QCP_STATES_VEL_KNEE->graph(0)->setName("desired knee vel");
    ui->QCP_STATES_VEL_KNEE->addGraph();
    ui->QCP_STATES_VEL_KNEE->graph(1)->setPen(myPen);
    ui->QCP_STATES_VEL_KNEE->graph(1)->setName("current knee vel");

    ui->QCP_STATES_TAU_HIP->legend->setVisible(true);
    ui->QCP_STATES_TAU_HIP->legend->setFont(QFont("Helvetica", 9));
    ui->QCP_STATES_TAU_HIP->addGraph();
    ui->QCP_STATES_TAU_HIP->graph(0)->setPen(dashPen);
    ui->QCP_STATES_TAU_HIP->graph(0)->setName("desired hip tau");
    ui->QCP_STATES_TAU_HIP->addGraph();
    ui->QCP_STATES_TAU_HIP->graph(1)->setPen(myPen);
    ui->QCP_STATES_TAU_HIP->graph(1)->setName("current hip tau");

    ui->QCP_STATES_TAU_KNEE->legend->setVisible(true);
    ui->QCP_STATES_TAU_KNEE->legend->setFont(QFont("Helvetica", 9));
    ui->QCP_STATES_TAU_KNEE->addGraph();
    ui->QCP_STATES_TAU_KNEE->graph(0)->setPen(dashPen);
    ui->QCP_STATES_TAU_KNEE->graph(0)->setName("desired knee tau");
    ui->QCP_STATES_TAU_KNEE->addGraph();
    ui->QCP_STATES_TAU_KNEE->graph(1)->setPen(myPen);
    ui->QCP_STATES_TAU_KNEE->graph(1)->setName("current knee tau");


    ui->QCP_GRF_ERROR->xAxis->setTicker(timeTicker);
    ui->QCP_GRF_GRF->xAxis->setTicker(timeTicker);
    ui->QCP_STATES_POS_HIP->xAxis->setTicker(timeTicker);
    ui->QCP_STATES_POS_KNEE->xAxis->setTicker(timeTicker);
    ui->QCP_STATES_VEL_HIP->xAxis->setTicker(timeTicker);
    ui->QCP_STATES_VEL_KNEE->xAxis->setTicker(timeTicker);
    ui->QCP_STATES_TAU_HIP->xAxis->setTicker(timeTicker);
    ui->QCP_STATES_TAU_KNEE->xAxis->setTicker(timeTicker);

    ui->QCP_GRF_ERROR->xAxis->setRange(-10, 10);
    ui->QCP_GRF_GRF->xAxis->setRange(-10, 10);
    ui->QCP_STATES_POS_HIP->xAxis->setRange(-10, 10);
    ui->QCP_STATES_POS_KNEE->xAxis->setRange(-10, 10);
    ui->QCP_STATES_VEL_HIP->xAxis->setRange(-10, 10);
    ui->QCP_STATES_VEL_KNEE->xAxis->setRange(-10, 10);
    ui->QCP_STATES_TAU_HIP->xAxis->setRange(-10, 10);
    ui->QCP_STATES_TAU_KNEE->xAxis->setRange(-10, 10);

    ui->QCP_GRF_ERROR->yAxis->setRange(-10, 10);
    ui->QCP_GRF_GRF->yAxis->setRange(0, 30);
    ui->QCP_STATES_POS_HIP->yAxis->setRange(0, 3);
    ui->QCP_STATES_POS_KNEE->yAxis->setRange(0, -3);
    ui->QCP_STATES_VEL_HIP->yAxis->setRange(-10, 10);
    ui->QCP_STATES_VEL_KNEE->yAxis->setRange(-10, 10);
    ui->QCP_STATES_TAU_HIP->yAxis->setRange(-10, 10);
    ui->QCP_STATES_TAU_KNEE->yAxis->setRange(-10, 10);
}

void MainWindow::displayUpdate()
{
    for(int index = 0; index < MOTOR_NUM; index++)
    {
        if(sharedMemory->motorStatus)
        {
            ui->tableWidget->item(index, 0)->setBackgroundColor(QColor(75, 75, 255));
        }
        else
        {
            ui->tableWidget->item(index, 0)->setBackgroundColor(QColor(255, 75, 75));
        }

        ui->tableWidget->item(index, 1)->setText(QString().sprintf("%d", sharedMemory->motorErrorStatus[index]));
        if(sharedMemory->motorErrorStatus[index] != 0)
        {
            ui->tableWidget->item(index, 1)->setBackgroundColor(QColor(255, 100, 100));
        }
        else
        {
            ui->tableWidget->item(index, 1)->setBackgroundColor(QColor(100, 255, 100));
        }
        ui->tableWidget->item(index, 2)->setText(QString().sprintf("%.1f", sharedMemory->motorVoltage[index]));
        ui->tableWidget->item(index, 3)->setText(QString().sprintf("%.1f", sharedMemory->motorPosition[index] * R2D));
        ui->tableWidget->item(index, 4)->setText(QString().sprintf("%.1f", sharedMemory->motorDesiredTorque[index]));
        ui->tableWidget->item(index, 5)->setText(QString().sprintf("%.1f", sharedMemory->motorTorque[index]));
    }
}

//TODO
void MainWindow::graphUpdate()
{

}

void MainWindow::initTable(QTableWidget* table)
{
    QFont tableFont;
    tableFont.setPointSize(7);

    const int col_num = 6;
    const int row_num = 2;
    const int col_width = 21;
    const int item_height = 25;
    const int item_width = 30;
    const int low_height = 25;

    // Horizontal - Column
    for(int i=0; i<col_num; i++)
    {
        table->insertColumn(i);
        table->setHorizontalHeaderItem(i, new QTableWidgetItem());
        table->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        table->horizontalHeaderItem(i)->setFont(tableFont);
        table->horizontalHeaderItem(i)->setSizeHint(QSize(col_width, item_height));
        table->setColumnWidth(i, col_width);
    }

    table->horizontalHeaderItem(0)->setText("Motor ID");
    table->horizontalHeaderItem(1)->setText("Error");
    table->horizontalHeaderItem(2)->setText("Voltage");
    table->horizontalHeaderItem(3)->setText("Angle\nMotor");
    table->horizontalHeaderItem(4)->setText("Torque\nDes");
    table->horizontalHeaderItem(5)->setText("Torque\nMotor");

    // Vertical - Row
    for(int i=0; i<row_num; i++)
    {
        table->insertRow(i);
        table->setRowHeight(i,low_height);
        table->setVerticalHeaderItem(i, new QTableWidgetItem());
        table->verticalHeaderItem(i)->setSizeHint(QSize(item_width, item_height));
        table->verticalHeaderItem(i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        table->verticalHeaderItem(i)->setFont(tableFont);
    }
    table->verticalHeaderItem(HIP_IDX)->setText("HIP");
    table->verticalHeaderItem(KNEE_IDX)->setText("KNEE");


    for(int i=0; i<row_num; i++)
    {
        for(int j=0; j<col_num; j++)
        {
            table->setItem(i, j, new QTableWidgetItem());
            table->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
            table->item(i,j)->setFlags(table->item(i,j)->flags() & ~Qt::ItemIsEditable);
            table->item(i,j)->setFont(tableFont);
        }
    }
    table->setMinimumHeight(low_height*(row_num+2));
    table->setMaximumHeight(low_height*(row_num+2));

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    table->item(0, 0)->setText(QString().sprintf("0x%x", MOTOR_HIP_ID));
    table->item(1, 0)->setText(QString().sprintf("0x%x", MOTOR_KNEE_ID));
}

//TODO:
void MainWindow::on_BT_MAIN_PANEL_START_SIMULATION_clicked()
{

}

void MainWindow::on_BT_MAIN_PANEL_RESET_clicked()
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
