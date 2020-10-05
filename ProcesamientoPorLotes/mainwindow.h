#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QVector>
#include <cmath>
#include <QDebug>
#include <QMessageBox>
#include <QTableWidget>
#include <QKeyEvent>
#include <QFuture>
#include <QtConcurrent>
#include <QThread>
#include <QFutureWatcher>

#include <random>
#include <unistd.h> // sleep (unix)

#include "Batch.h"
#include "Process.h"
#include "ThreadGlobalCounter.h"
#include "ThreadTImeElapsed.h"
#include "ThreadTImeLeft.h"
#include "ThreadBatchCounter.h"
#include "ThreadCurrentTableBatch.h"
#include "ThreadTableRunning.h"
#include "ThreadTableFinish.h"

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

    void test();

    QFuture<void> futureTableCurrentBatch;
    QFuture<void> futureTableProcessRunning;
    QFuture<void> futureTableProcessFinished;
    QFuture<void> futureGlobalCounter;
    QFuture<void> futureBatchCounter;
    QFuture<void> futureTimeElapsed;
    QFuture<void> futureTimeLeft;

    ThreadGlobalCounter *threadGlobalCounter;
    ThreadTImeElapsed *threadTimeElapsed;
    ThreadTImeLeft *threadTimeLeft;
    ThreadBatchCounter *threadBatchCounter;
    ThreadCurrentTableBatch *threadCurrentTableBatch;
    ThreadTableRunning *threadTableRunning;
    ThreadTableFinish *threadTableFinish;

    const int LIMITE_PROCESO = 4;

    bool errorOperation;
    bool errorID;
    bool firstTime;
    bool onlyOnce;
    bool randomData; // new
    int processInserted;
    int processRemaining;
    int batchNum;
    int indexBatch;
    int aux;
    int id; // new
    int globalCounter; // NEW

    void removeSpace(std::string& operation);
//    void runThreads();
    void insertProcessByUser(int& index); // new
    void insertProcessRandomly(int& index); // new
    void updateTableCurrentBatch(QList<Batch*>);
    void insertDataTableRunningProcess(QList<Batch*>);
    void updateTableFinish(QList<Batch*>);
    void reset();
    void updateGlobalCounter(QList<Batch*>);
    void updateBatchCounter(QList<Batch*>);
    void updateTimeElapsed(QList<Batch*>);
    void updateTimeLeft(QList<Batch*>);
    int getOperatorPos(const std::string& operation);
    int getOperandPos(const std::string& operation);
    int getLeftOperand(const std::string& operation);
    int getRightOperand(const std::string& operation);
    int computeBatches(int numProcesses); // new
    long doOperation(std::string& operation); // changed
    bool validID(int id);


protected:
    void keyPressEvent(QKeyEvent* event) override; // new

private slots:
    void sendData();
//    void updateGlobalCounter(int value);
//    void updateTimeElapsed(int value);
//    void updateTimeLeft(int value);
//    void updateTableFinish(Process *process);
//    void updateTableCurrentBatch(Batch *batch);
    void insertDataTableCurrentBatch();
//    void insertDataTableRunningProcess(Process* runningProcess);
//    void reset();
//    void updateBatchCounter(int value);
    void on_action_Procesar_Lote_triggered();
    void on_action_Procesar_Lote_con_Informacion_Aleatoria_triggered();
};
#endif // MAINWINDOW_H
