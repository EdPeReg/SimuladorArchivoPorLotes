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
#include <QEventLoop>
#include <QTimer>

#include <random>
#include <deque>
#include <vector>

//#include "Batch.h"
#include "Process.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum CURRENT_BATCH {
    ID, TME, TT
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

    std::deque<Process> nuevos;
    std::deque<Process> listos;
    std::deque<Process> bloqueados;
    std::vector<Process> terminados;

//    QList< std::deque<Process> > queues;

//    QList<Batch> batches;
//    QList<Batch> batchesCopy;

    const int LIMITE_PROCESO = 4;

    bool errorOperation;
    bool errorID;
    bool notFirstPauseTime;
    bool pauseRequired;
    bool keyError;
    bool IO_interruptionKey;
    int processInserted;
    int processRemaining;
    int batchNum;
    int id;
    int globalCounter;

    void removeSpace(std::string& operation);
    void insertProcessRandomly();
    inline void delay(int millisecondsWait);
    void pause();
    void resume();
    void updateGlobalCounter(int value);
    void updateTableFinish(Process process);
    void runWithRandomData();
    void updateTableCurrentBatch(const std::deque<Process>&deque, int& row);
    void updateTT_TR_counters(int& counterTimeElapsed, int& counterTimeLeft);
    void insertDataTableRunningProcess(Process runningProcess);
    void reset();
    void updateProcCounter(int value);
    void deleteProcessesNuevo();
    std::deque<Process> slice(const std::deque<Process> &deque);
    int getOperatorPos(const std::string& operation);
    int getOperandPos(const std::string& operation);
    int computeQueues(int numProcess);
    int getLeftOperand(const std::string& operation);
    int getRightOperand(const std::string& operation);
    long doOperation(std::string& operation);
protected:
    void keyPressEvent(QKeyEvent* event) override; // new

private slots:
    void sendData();
    void on_action_Procesar_Procesos_triggered();
};
#endif // MAINWINDOW_H
