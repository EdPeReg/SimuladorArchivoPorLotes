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

#include "Batch.h"
#include "Process.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct SaveState {
    int batchIndex = 0;
    int batchCounter = 0;
    int indexProcess = 0;
    int counterTimeElapsed = 0;
    int counterTimeLeft = 0;
    int counter = 0;
    int TT = 0;
    int TME = 0;
};

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

    QList<Batch*> batches;
    QList<Batch *> batchesCopy;
    QVector<int> ids;
    Process *process;
    SaveState saveState;

    const int LIMITE_PROCESO = 4;

    bool errorOperation;
    bool errorID;
    bool notFirstPauseTime;
    bool onlyOnce;
    bool randomData; // new
    bool pauseRequired;
    bool keyError;
    bool IO_interruptionKey;
    bool notFirstPause;
    int processInserted;
    int processRemaining;
    int batchNum;
    int batchesRemaining;
    int indexBatch;
    int aux;
    int id; // new
    int globalCounter;

    void removeSpace(std::string& operation);
    void insertProcessByUser(int& index); // new
    void insertProcessRandomly(int& index); // new
    inline void delay(int millisecondsWait); // new
    void pause(); // new
    void resume(); // new
    void updateGlobalCounter(int value);
    void updateTableFinish(Process *process);
    void updateTableCurrentBatch();
    void insertDataTableCurrentBatch();
    void insertDataTableRunningProcess(Process* runningProcess);
    void reset();
    void updateBatchCounter(int value);
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
    void on_action_Procesar_Lote_triggered();
    void on_action_Procesar_Lote_con_Informacion_Aleatoria_triggered();
};
#endif // MAINWINDOW_H
