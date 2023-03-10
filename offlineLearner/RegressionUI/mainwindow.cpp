#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 *
 * mDatasetColum        : Dataset's dimension. (input dim. + output dim.)
 * mDatasetRow          : Number of the datasets.
 * mValidationsetColum  : Validation dataset's dimension. (input dim. + output dim.)
 * mValidationsetRow    : Number of the validation datasets.
 * mInputDimension      : Dimension of input of the neural network.
 * mOutputDimension     : Dimension of output of the neural network.
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mBatchSize(DEFAULT_BATCH_SIZE)
    , mDatasetColum(15)
    , mDatasetRow(620354)
    , mValidationsetColum(15)
    , mValidationsetRow(10000)
    , mInputDimension(14)
    , mOutputDimension(1)
    , mEpochs(DEFAULT_EPOCHS)
    , mCurrentEpoch(0)
    , mEstimateIndex(0)
    , mEstimateRange(1000)
    , mLearningRate(DEFAULT_LEARNING_RATE)
    , mMiniBatchLoss(0.0)
    , mMiniBatchLossMin(0.0)
    , mMiniBatchLossMax(0.0)
    , mLossMean(0.0)
    , mMSE(0.0)
    , mEstimatedValue(0.0)
    , mGroundTruthValue(0.0)
{
    ui->setupUi(this);
    initializeDisplay();
    initializeGraph();

    net = std::make_shared<Net>(mInputDimension, mOutputDimension);
    mDataSet = torch::randn({ mDatasetRow, mDatasetColum });
    mValidationSet = torch::randn({ mValidationsetRow, mValidationsetColum });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeDisplay()
{
    ui->LE_DATASET_LOAD_PATH->setText("GRFDatasets.csv");
    ui->LE_VALIDATION_LOAD_PATH->setText("GRFDatasets_valid.csv");
    ui->LE_PARAMS_BATCH_SIZE->setText(QString().sprintf("%d", mBatchSize));
    ui->LE_PARAMS_EPOCHS->setText(QString().sprintf("%d", mEpochs));
    ui->LE_PARAMS_LAERNING_RATE->setText(QString().sprintf("%f", mLearningRate));
    ui->LE_MODEL_SAVE_PATH->setText("Model.pt");
    ui->LE_MODEL_LOAD_PATH->setText("Model.pt");
}

void MainWindow::initializeGraph()
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

    ui->QCP_LEARNING->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP :: iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->QCP_ESTIMATE->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP :: iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);

    ui->QCP_LEARNING->legend->setVisible(true);
    ui->QCP_LEARNING->legend->setFont(QFont("Helvetica", 9));
    ui->QCP_LEARNING->addGraph();
    ui->QCP_LEARNING->graph(0)->setPen(dotPen);
    ui->QCP_LEARNING->graph(0)->setName("Loss - Band");
    ui->QCP_LEARNING->graph(0)->setBrush(QBrush(QColor(255,50,30,20)));
    ui->QCP_LEARNING->addGraph();
    ui->QCP_LEARNING->legend->removeItem(ui->QCP_LEARNING->legend->itemCount()-1);
    ui->QCP_LEARNING->graph(1)->setPen(dotPen);
    ui->QCP_LEARNING->graph(0)->setChannelFillGraph(ui->QCP_LEARNING->graph(1));
    ui->QCP_LEARNING->addGraph();
    ui->QCP_LEARNING->graph(2)->setPen(myPen);
    ui->QCP_LEARNING->graph(2)->setName("Loss - Mean");

    ui->QCP_ESTIMATE->legend->setVisible(true);
    ui->QCP_ESTIMATE->legend->setFont(QFont("Helvetica", 9));
    ui->QCP_ESTIMATE->addGraph();
    ui->QCP_ESTIMATE->graph(0)->setPen(myPen);
    ui->QCP_ESTIMATE->graph(0)->setName("Estimated value");
    ui->QCP_ESTIMATE->addGraph();
    ui->QCP_ESTIMATE->graph(1)->setPen(dashPen);
    ui->QCP_ESTIMATE->graph(1)->setName("Ground-true");

    ui->QCP_LEARNING->xAxis->setRange(0, mEpochs);
    ui->QCP_ESTIMATE->xAxis->setRange(0, mEstimateRange);
    ui->QCP_LEARNING->yAxis->setRange(0, 10);
    ui->QCP_ESTIMATE->yAxis->setRange(0, 30);
}

void MainWindow::updateDisplay()
{
    ui->LE_LEARNING_MINI_BATCH_LOSS->setText(QString().sprintf("%f", mMiniBatchLoss));
    ui->LE_ESTIMATE_MSE->setText(QString().sprintf("%f", mMSE));
}

void MainWindow::updateLearningGraph()
{
    ui->QCP_LEARNING->graph(0)->addData(mCurrentEpoch, mMiniBatchLossMax);
    ui->QCP_LEARNING->graph(1)->addData(mCurrentEpoch, mMiniBatchLossMin);
    ui->QCP_LEARNING->graph(2)->addData(mCurrentEpoch, mLossMean);
    ui->QCP_LEARNING->xAxis->setRange(1, mEpochs);
    ui->QCP_LEARNING->replot();
}

void MainWindow::updateValidationGraph(int* range)
{
    ui->QCP_ESTIMATE->graph(0)->addData(mEstimateIndex, mEstimatedValue);
    ui->QCP_ESTIMATE->graph(1)->addData(mEstimateIndex, mGroundTruthValue);
    ui->QCP_ESTIMATE->xAxis->setRange(range[0], range[1]);
    ui->QCP_ESTIMATE->replot();
}

void MainWindow::on_BT_DATASET_LOAD_clicked()
{
    std::string dataPath;
    dataPath.append(DATASET_DIR);
    dataPath.append(ui->LE_DATASET_LOAD_PATH->text().toStdString());
    loadData(mDataSet, dataPath);
}

void MainWindow::loadData(torch::Tensor& tensor, std::string& dataPath)
{
    std::ifstream ifs(dataPath);
    std::vector<float> datasets;
    try
    {
        if (!ifs.is_open())
        {
            throw dataPath;
        }
        float x = 0;
        char dummy;

        for (int i = 0; i < tensor.size(0); ++i)
        {
            for (int i = 0; i < tensor.size(1); ++i)
            {
                ifs >> x;
                datasets.push_back(x);
                if (i < (tensor.size(1) - 1))
                {
                    ifs >> dummy;
                }
            }
        }
        mVectorDatasets = datasets;
        ifs.close();
        vec2tensor(tensor);
        std::cout << "[DATA LOADER] Datasets are loaded." << std::endl << std::endl;;
    }
    catch (std::string path)
    {
        perror("[DATA LOADER] Datasets are not loaded.");
        std::cout << "[DATA LOADER] Given path : " << path << std::endl << std::endl;
    }
}

void MainWindow::vec2tensor(torch::Tensor& tensor)
{
    auto tensorAccessor = tensor.accessor<float, 2>();

    for (int i = 0; i < tensor.size(0); i++)
    {
        for(int j = 0; j < tensor.size(1) ; j++)
        {
            tensorAccessor.data()[i * tensor.size(1) + j] = mVectorDatasets[i * tensor.size(1) + j];
        }
    }
}

void MainWindow::doValidation(int* range)
{
    float squaredError = 0;
    float sumOfSquaredError = 0;
    auto inputs = torch::randn({ 1, mInputDimension });
    auto inputTensorAccessor = inputs.accessor<float, 2>();

    ui->QCP_ESTIMATE->graph(0)->data()->clear();
    ui->QCP_ESTIMATE->graph(1)->data()->clear();

    for(int i = range[0] ; i < range[1]+1 ; i++)
    {
        for( int j = 0 ; j < mInputDimension ; j++)
        {
            inputTensorAccessor.data()[j] = mValidationSet.data()[i].data()[j].item<float>();
        }
        mGroundTruthValue = mValidationSet.data()[i].data()[mInputDimension].item<float>();
        mEstimateIndex = i;
        mEstimatedValue = net->forward(inputs).item<float>();
        squaredError =  pow((mGroundTruthValue - mEstimatedValue), 2.0);
        sumOfSquaredError += squaredError;
        mMSE = squaredError / (i + 1 - range[0]);
        updateDisplay();
        updateValidationGraph(range);
        std::cout << "[ESTIMATION] Index : " << mEstimateIndex << "\tSquared Error : " << squaredError << "\tEstimated Value : " << mEstimatedValue << "\tGround-true Value : " << mGroundTruthValue << std::endl;
    }
    std::cout << "[ESTIMATION] RMSE : " << pow((sumOfSquaredError / (range[1] - range[0] + 1)) , 0.5) << std::endl << std::endl;
}

void MainWindow::on_BT_LEARNING_clicked()
{
    ui->QCP_LEARNING->graph(0)->data()->clear();
    ui->QCP_LEARNING->graph(1)->data()->clear();
    ui->QCP_LEARNING->graph(2)->data()->clear();

    mBatchSize = ui->LE_PARAMS_BATCH_SIZE->text().toInt();
    mLearningRate = ui->LE_PARAMS_LAERNING_RATE->text().toFloat();
    mEpochs = ui->LE_PARAMS_EPOCHS->text().toInt();
    std::cout << "[PARAMETERS] Batch size\t\t : " << mBatchSize << std::endl;
    std::cout << "[PARAMETERS] Learning rate\t : " << mLearningRate << std::endl;
    std::cout << "[PARAMETERS] Epochs\t\t\t : " << mEpochs << std::endl;

    torch::optim::Adam optimizer(net->parameters(), torch::optim::AdamOptions(mLearningRate));
    auto inputs = torch::randn({ mBatchSize, mInputDimension });
    auto target = torch::randn({ mBatchSize, mOutputDimension });
    auto datasets = torch::data::datasets::TensorDataset(mDataSet);
    auto inputTensorAccessor = inputs.accessor<float, 2>();
    auto targetTensorAccessor = target.accessor<float, 2>();

    for (std::size_t epoch = 1; epoch <= mEpochs; epoch++)
    {
        int iteration = 0;
        mLossMean = 0;
        mMiniBatchLossMin = 10000;
        mMiniBatchLossMax = 0;
        auto dataLoader = torch::data::make_data_loader(datasets, torch::data::DataLoaderOptions().batch_size(mBatchSize));
        for (auto& batch : *dataLoader)
        {
            optimizer.zero_grad();

            for (int64_t i = 0; i < batch.size(); ++i)
            {
                for (int j = 0; j < 14 ; j++)
                {
                    inputTensorAccessor.data()[i * 14 + j] = batch.data()[i].data[j].item<float>();
                }
                targetTensorAccessor.data()[i] = batch.data()[i].data[mInputDimension].item<float>();
            }
            auto out = net->forward(inputs);
            auto loss = torch::mse_loss(out, target);
            mMiniBatchLoss = loss.item<float>();
            loss.backward();
            optimizer.step();
            mLossMean += mMiniBatchLoss;
            iteration++;
            if(mMiniBatchLoss < mMiniBatchLossMin)
            {
                mMiniBatchLossMin = mMiniBatchLoss;
            }
            if(mMiniBatchLoss > mMiniBatchLossMax)
            {
                mMiniBatchLossMax = mMiniBatchLoss;
            }
        }
        mLossMean /= iteration;
        mCurrentEpoch = epoch;
        std::cout << "[LEARNING] Epoch : " << mCurrentEpoch << "\tAverage Loss : " << mLossMean << std::endl;
        updateDisplay();
        updateLearningGraph();
    }
    std::cout << std::endl;
}

void MainWindow::on_BT_MODEL_SAVE_clicked()
{
    std::cout << "Model save is clicked." << std::endl;
    std::string modelPath;
    modelPath.append(MODEL_DIR);
    modelPath.append(ui->LE_MODEL_SAVE_PATH->text().toStdString());
    torch::save(net, modelPath);
    std::cout << "[MODEL MANAGER] : " << ui->LE_MODEL_SAVE_PATH->text().toStdString() << " is saved." << std::endl << std::endl;
}

void MainWindow::on_BT_MODEL_LOAD_clicked()
{
    std::string modelPath;
    modelPath.append(MODEL_DIR);
    modelPath.append(ui->LE_MODEL_LOAD_PATH->text().toStdString());
    std::ifstream ifs(modelPath);
    try
    {
        if(!ifs.is_open())
        {
            throw modelPath;
        }
        torch::load(net, modelPath);
        std::cout << "[MODEL MANAGER] : " << ui->LE_MODEL_LOAD_PATH->text().toStdString() << " is loaded." << std::endl << std::endl;
    }
    catch(std::string path)
    {
        perror("[MODEL MANAGER] Model is not loaded.");
        std::cout << "[MODEL MANAGER] Given path : " << path << std::endl << std::endl;
    }
    ifs.close();
}

void MainWindow::on_BT_VALIDATION_ESTIMATE_clicked()
{
    int idx[2];
    idx[0] = ui->LE_VALIDATION_START->text().toInt();
    idx[1] = ui->LE_VALIDATION_END->text().toInt();
    try
    {
        if((idx[0] < 0) || (idx[0] > mValidationsetRow-1) || (idx[0] > idx[1]) || (idx[1] > mValidationsetRow-1))
        {
            throw idx;
        }
        doValidation(idx);
    }
    catch(int* idx)
    {
        perror("[MODEL VALIDATION] Validation set's Index numbers are invalid.");
        std::cout<<"[MODEL VALIDATION] Index numbers should be in 0 ~ "<<mValidationsetRow-1<<"."<<std::endl;
    }
}

void MainWindow::on_BT_VALIDATION_LOAD_clicked()
{
    std::string dataPath;
    dataPath.append(VALIDSET_DIR);
    dataPath.append(ui->LE_VALIDATION_LOAD_PATH->text().toStdString());
    loadData(mValidationSet, dataPath);
}

void MainWindow::on_BT_RESET_NETWORK_clicked()
{
    net = std::make_shared<Net>(mInputDimension, mOutputDimension);
}
