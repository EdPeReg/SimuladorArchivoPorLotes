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
#include "DialogCounters.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum COLUMN_CURRENT_BATCH {
    ID, TME
};;

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
    void getTMEProcess();
    void insertDataTableCurrentBatch();

public:
    QVector<Batch *> getBatches() const;

private slots:
    void sendData();

    void on_action_Procesar_Lote_triggered();
};
#endif // MAINWINDOW_H
