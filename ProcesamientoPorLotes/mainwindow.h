#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <QMainWindow>
#include <QVector>
#include <cmath>
#include <QDebug>
#include <QMessageBox>
#include <QStyle>

#include "Batch.h"
#include "Process.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QVector<Batch*> batches;

    const int LIMITE_PROCESO = 4;

    bool errorOperation;
    bool errorID;
    bool firstTime;
    bool onlyOnce;
    int processInserted;
    int processRemaining;
    int batchesCount;
    int batchNum;
    int indexBatch;

    void removeSpace(std::string& operation);
    void insertProcess(int& index);
    int getOperandPos(const std::string& operation);
    int getLeftOperand(const std::string& operation);
    int getRightOperand(const std::string& operation);
    int computebatcheses(int numProcess);
    int doOperation(std::string& operation);
    bool validID(int id);


private slots:
    void sendData();


    void on_action_Procesar_Lote_triggered();
};
#endif // MAINWINDOW_H
