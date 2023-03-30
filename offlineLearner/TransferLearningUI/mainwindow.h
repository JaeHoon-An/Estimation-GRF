#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <torch/torch.h> // "torch.h" should be included in first.
#include <QMainWindow>
#include <vector>
#include <iostream>

constexpr int DEFAULT_BATCH_SIZE = 32;
constexpr int DEFAULT_EPOCHS = 100;
constexpr float DEFAULT_LEARNING_RATE = 0.001;


QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_BT_MODEL_SAVE_clicked();
    void on_BT_MODEL_LOAD_clicked();
    void on_BT_DATASET_LOAD_clicked();
    void on_BT_LEARNING_clicked();
    void on_BT_VALIDATION_LOAD_clicked();
    void on_BT_VALIDATION_ESTIMATE_clicked();
    void on_BT_RESET_NETWORK_clicked();

private:
    struct swish : torch::nn::Module
    {
        torch::Tensor forward(torch::Tensor x)
        {
            x = x * torch::sigmoid(x);
            return x;
        }
    };
    struct Sim2RealNet : torch::nn::Module
    {
        torch::nn::Linear fc1{ nullptr }, fc2{ nullptr }, fc3{ nullptr }, fc4{ nullptr }, fc5{ nullptr }, fc6{ nullptr };
        swish activationSwish;

        Sim2RealNet(int in_dim, int out_dim)
        {
            fc1 = register_module("fc1", torch::nn::Linear(in_dim, 16));
            fc2 = register_module("fc2", torch::nn::Linear(16, 16));
            fc3 = register_module("fc3", torch::nn::Linear(16, 16));
            fc4 = register_module("fc4", torch::nn::Linear(16, 16));
            fc5 = register_module("fc5", torch::nn::Linear(16, 16));
            fc6 = register_module("fc6", torch::nn::Linear(16, out_dim));
        }

        torch::Tensor forward(torch::Tensor x)
        {
            x = fc1->forward(x);
            x = fc2->forward(activationSwish.forward(x));
            x = fc3->forward(activationSwish.forward(x));
            x = fc4->forward(activationSwish.forward(x));
            x = fc5->forward(activationSwish.forward(x));
            x = fc6->forward(activationSwish.forward(x));
            return x;
        }
    };
    void initializeDisplay();
    void initializeGraph();
    void updateDisplay();
    void updateLearningGraph();
    void updateValidationGraph(int* range);
    void loadData(torch::Tensor& tensor, std::string& dataPath);
    void vec2tensor(torch::Tensor& tensor);
    void doValidation(int* range);

    Ui::MainWindow* ui;
    torch::Tensor mDataSet;
    torch::Tensor mValidationSet;
    std::vector<float> mVectorDatasets;
    int mBatchSize;
    int mDatasetColum;
    int mDatasetRow;
    int mValidationsetColum;
    int mValidationsetRow;
    int mInputDimension_Sim2RealNet;
    int mOutputDimension_Sim2RealNet;
    int mEpochs;
    int mCurrentEpoch;
    int mEstimateIndex;
    int mEstimateRange;
    float mLearningRate;
    float mMiniBatchLoss;
    float mMiniBatchLossMin;
    float mMiniBatchLossMax;
    float mLossMean;
    float mMSE;
    float mEstimatedValue;
    float mGroundTruthValue;
    std::shared_ptr<Sim2RealNet> Sim2Realnet;
};

#endif // MAINWINDOW_H
