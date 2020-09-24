#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QVector>
#include <cmath>
#include <QDebug>
#include <QMessageBox>
#include <QTableWidget>

#include "Batch.h"
#include "Process.h"
#include "ThreadGlobalCounter.h"
#include "ThreadProcessRunning.h"
#include "ThreadTImeElapsed.h"
#include "ThreadTImeLeft.h"
#include "ThreadBatchCounter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum CURRENT_BATCH {
    ID, TME
};;

enum RUNNING_PROCESS {
    ID_RP, NOMBRE_RP, OPERACION_RP, TME_RP, TT_RP, TR_RP
};

enum FINISH_PROCESS{
    ID_FP, NOMBRE_FP, OPERACION_FP, RESULT_FP, TME_FP, LOTE_FP
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QList<Batch*> batches;
    Process *process;
    QVector<int> ids;
    ThreadGlobalCounter *threadGlobalCounter;
    ThreadProcessRunning *threadProcessRunning;
    ThreadTImeElapsed *threadTimeElapsed;
    ThreadTImeLeft *threadTimeLeft;
    ThreadBatchCounter *threadBatchCounter;

    const int LIMITE_PROCESO = 4;

    bool errorOperation;
    bool errorID;
    bool firstTime;
    bool onlyOnce;
    int processInserted;
    int processRemaining;
    int batchNum;
    int indexBatch;
    int aux;

    void removeSpace(std::string& operation);
    void insertProcess(int& index);
    void runThreads(); // new;
    int getOperatorPos(const std::string& operation);
    int getOperandPos(const std::string& operation);
    int getLeftOperand(const std::string& operation);
    int getRightOperand(const std::string& operation);
    int doOperation(std::string& operation);
    bool validID(int id);

private slots:
    void sendData();
    void updateGlobalCounter(int value);
    void updateTimeElapsed(int value);
    void updateTimeLeft(int value);
    void updateTableFinish(Process *process);
    void updateTableCurrentBatch(Batch *batch);
    void insertDataTableCurrentBatch();
    void insertDataTableRunningProcess(Process* runningProcess);
    void reset();
    void updateBatchCounter(int value);
    void on_action_Procesar_Lote_triggered();
};
#endif // MAINWINDOW_H
