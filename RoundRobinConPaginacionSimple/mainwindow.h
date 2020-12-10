#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
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

#include "Process.h"
#include "ProcessesDialog.h"
#include "dialogQuantum.h"
#include "Memory.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum CURRENT_BATCH {
    ID, TME, TT, TAMANO
};;

enum RUNNING_PROCESS {
    ID_RP, NOMBRE_RP, OPERACION_RP, TME_RP, TT_RP, TR_RP, QUANT_RP
};

enum FINISH_PROCESS{
    ID_FP, NOMBRE_FP, OPERACION_FP, RESULT_FP, TME_FP, LOTE_FP
};

enum BLOQUEADO_PROCESS {
    ID_BP, TTB_BP
};

/* We use this to know the row start and the process, to insert it to the proper row
 * position table. */
struct Utility{
    int processID;
    int startRow;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ProcessesDialog* processesDialog;
    dialogQuantum* quantumValueDialog;
    Process process;
    Memory memory;
    std::deque<Process> nuevos;
    std::deque<Process> listos;
    std::deque<Process> bloqueados;
    std::vector<Process> allProcesses;
    std::vector<Process> terminados;
    std::vector<Utility> utilidades;

    const int LIMITE_TTB = 8;

    bool pauseRequired;
    bool keyError;
    bool IO_interruptionKey;
    bool isProcessNull;
    bool keyN_pressed;
    int processInserted;
    int processesRemaining;
    int processNum;
    int id;
    int globalCounter;
    int nuevosIndex;
    int nuevosdequeSize;
    int counterTimeElapsed;
    int counterTimeLeft;
    int quantumValue; // NEW
    int processesLeft;
    short row; // new

    void removeSpace(std::string& operation);
    void insertProcessRandomly();
    void insertProcess();
    inline void delay(int millisecondsWait);
    void pause();
    void resume();
    void updateGlobalCounter(int value);
    void updateTableFinish(const Process& process);
    void runWithRandomData();
    void updateTableNuevos();
    void updateTableListos(const std::deque<Process>&deque, int& row);
    void insertLastTableListo(const Process& process);
    void insertLastTableNuevo(const Process& process);
    void insertDataTableNuevo(const std::deque<Process>& nuevos);
    void insertTableMemory(std::deque<Process> listos, std::deque<Process> bloqueados); // NEW
    void insertRunningProcessMemory(const Process& process); // New
    void insertListosMemory(const Process& process); // new
    void insertBloqueadosMemory(const Process& process); // new
    void setColorOS(int row, int col); // new
    void setTableMemory();
    void clearRowMemory();
    void updateTT_TR_counters(int& counterTimeElapsed, int& counterTimeLeft);
    void updateTTBCounter();
    void updateQuantumValue();
    void updateBloqueadosTable(const Process& process);
    void insertDataTableRunningProcess(const Process& runningProcess);
    void reset();
    void updateProcCounter(int value);
    void deleteProcessesNuevo();
    void setInitialProcCounterValue();
    void setNullProcess();
    int getOperatorPos(const std::string& operation);
    int getOperandPos(const std::string& operation);
    int computeQueues(int numProcess);
    int getLeftOperand(const std::string& operation);
    int getRightOperand(const std::string& operation);
    QString doOperation(std::string& operation);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void sendData();
    void on_action_Procesar_Procesos_triggered();
};
#endif // MAINWINDOW_H
