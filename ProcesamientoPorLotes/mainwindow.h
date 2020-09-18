#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <QMainWindow>
#include <QVector>
#include <cmath>
#include <QDebug>
#include <QMessageBox>
#include <QTableWidget>
#include <stdexcept>

#include "Batch.h"
#include "Process.h"
#include "ThreadGlobalCounter.h"
#include "ThreadProcessRunning.h"
#include "ThreadTImeElapsed.h"
#include "DialogCounters.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum CURRENT_BATCH {
    ID, TME
};;

enum RUNNING_PROCESS {
    ID_RP, NOMBRE_RP, OPERACION_RP, TME_RP, TT_RP, TR_RP
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QVector<Batch*> batches;
    ThreadGlobalCounter *threadGlobalCounter;
    ThreadProcessRunning *threadProcessRunning;
    ThreadTImeElapsed *threadTimeElapsed;

    const int LIMITE_PROCESO = 4;

    bool errorOperation;
    bool errorID;
    bool firstTime;
    bool onlyOnce;
    int processInserted;
    int processRemaining;
    int batchNum;
    int indexBatch;
    int globalCounter;
    int indexCurrentBatch;

    void removeSpace(std::string& operation);
    void insertProcess(int& index);
    int getOperandPos(const std::string& operation);
    int getLeftOperand(const std::string& operation);
    int getRightOperand(const std::string& operation);
    int doOperation(std::string& operation);
    bool validID(int id);
    void updateGlobalCounter(int n);
    void updateTimeElapsed(int n);
    void getTMEProcess();
    void insertDataTableCurrentBatch();
    void insertDataTableRunningProcess(Process* runningProcess);

private slots:
    void sendData();

    void on_action_Procesar_Lote_triggered();
};
#endif // MAINWINDOW_H
