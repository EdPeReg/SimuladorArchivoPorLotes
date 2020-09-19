#include "mainwindow.h"
#include "ui_mainwindow.h"

// TODO
// BUSCAR MANERA DE EVITAR MEMORY LEAKS
// VALIDAR MQUE EL 0 FUNCIONE EN LAS DEMAS OPERACIONES .
//
// NOTES:
//
// I used a lot of vectors in each threads, I don't want to use only vectors to get only
// one element, find a better wa.
//
// Maybe I can create a thread only for update tables and another thread for counters.

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , threadGlobalCounter(nullptr)
    , threadProcessRunning(nullptr)
    , threadTimeElapsed(nullptr)
    , errorOperation(false)
    , errorID(false)
    , firstTime(false)
    , onlyOnce(false)
    , processInserted(0)
    , processRemaining(0)
    , batchNum(1)
    , indexBatch(0)
    , rows(6)
    , columns(0)
    , aux(0)
{
    ui->setupUi(this);

    threadGlobalCounter = new ThreadGlobalCounter;
    threadProcessRunning = new ThreadProcessRunning;
    threadTimeElapsed = new ThreadTImeElapsed;
    threadTimeLeft = new ThreadTImeLeft;

    connect(threadGlobalCounter, &ThreadGlobalCounter::updateCounter, this, &MainWindow::updateGlobalCounter);
    connect(threadProcessRunning, &ThreadProcessRunning::updateTable, this, &MainWindow::insertDataTableRunningProcess);
    connect(threadTimeElapsed, &ThreadTImeElapsed::updateCounter, this, &MainWindow::updateTimeElapsed);
    connect(threadTimeLeft, &ThreadTImeLeft::updateCounter, this, &MainWindow::updateTimeLeft);

    ui->tblWdt_LoteActual->setColumnCount(2);
    ui->tblWdt_LoteActual->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    ui->tblWdt_LoteActual->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("TME")));
    ui->tblWdt_LoteActual->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.

    ui->tblWdt_ProcesoEjec->setColumnCount(1);
    ui->tblWdt_ProcesoEjec->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("DATOS")));
    ui->tblWdt_ProcesoEjec->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.

    ui->tblWdt_ProcesoEjec->setRowCount(6);
    ui->tblWdt_ProcesoEjec->setVerticalHeaderItem(0, new QTableWidgetItem(("ID")));
    ui->tblWdt_ProcesoEjec->setVerticalHeaderItem(1, new QTableWidgetItem(("NOMBRE")));
    ui->tblWdt_ProcesoEjec->setVerticalHeaderItem(2, new QTableWidgetItem(("OPERACION")));
    ui->tblWdt_ProcesoEjec->setVerticalHeaderItem(3, new QTableWidgetItem(("TME")));
    ui->tblWdt_ProcesoEjec->setVerticalHeaderItem(4, new QTableWidgetItem(("TT")));
    ui->tblWdt_ProcesoEjec->setVerticalHeaderItem(5, new QTableWidgetItem(("TR")));

    ui->tblWdt_Terminados->setColumnCount(1);
    ui->tblWdt_Terminados->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("DATOS")));
    ui->tblWdt_Terminados->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.

    connect(ui->btn_Enviar, &QPushButton::clicked, this, &MainWindow::sendData);
}

MainWindow::~MainWindow()
{
    delete ui;

    for(QVector<Batch*>::iterator it = batches.begin(); it != batches.end(); ++it) {
        delete (*it);
    }

    delete threadGlobalCounter;
    delete threadProcessRunning;
    delete threadTimeElapsed;
    delete threadTimeLeft;
}

void MainWindow::removeSpace(std::string& operation) {
    std::string a;

    std::string toErase = " ";
    size_t pos = std::string::npos;
    while((pos = operation.find(toErase)) != std::string::npos) {
        operation.erase(pos, toErase.size());
    }
}

void MainWindow::insertProcess(int& index)
{
    Process* process = new Process;
    int rightOperand = 0;
    QString programmerName = ui->ldt_NombProgr->text();
    process->setProgrammerName(programmerName);

    QString aux = ui->ldt_Operacion->text();
    std::string operation = aux.toStdString();
    removeSpace(operation);

    rightOperand = getRightOperand(operation);
    if(rightOperand == 0) {
        QMessageBox::warning(this, tr("errorOperation, OPERACION INVALIDA"), tr("Operacion invalida, ingrese una operacion valida"));
        errorOperation = true;
        ui->ldt_Operacion->clear();
    } else {
        errorOperation = false;
    }

    if(!errorOperation) {
        process->setOperation(aux);
        process->setResult(doOperation(operation));

        int tiempoMaximoEst = ui->spnBx_TME->value();
        process->setTiempoMaximoEst(tiempoMaximoEst);

        int id = ui->spnBx_ID->value();

        if(validID(id)) {
            process->setId(id);
            batches.at(index)->insertProcess(process);

            if(++processInserted == LIMITE_PROCESO) {
                process->setNumBatch(batchNum);
                ui->lcd_LotesRestantes->display(batchNum++);
                processInserted = 0;
            } else {
                process->setNumBatch(batchNum);
            }
            qDebug() << " ";
            qDebug() << "Indice antes de insertar: " << index;
            qDebug() << "LOtes totales antes de insertar: " << batches.size();
        }
    }
}

int MainWindow::getOperandPos(const std::string& operation) {
    size_t posOperand = operation.find_first_of("*/m+-p");
    if(posOperand != std::string::npos) {
        return posOperand;
    }

    return -1;
}

int MainWindow::getLeftOperand(const std::string& operation) {
    int operandPos = getOperandPos(operation);
    return stoi(operation.substr(0, operandPos));
}

int MainWindow::getRightOperand(const std::string& operation) {
    int operandPos = getOperandPos(operation);
    return stoi(operation.substr(operandPos + 1));
}

int MainWindow::doOperation(std::string& operation) {
    int operandPos = getOperandPos(operation);
    int leftOperand = getLeftOperand(operation);
    int rightOperand = getRightOperand(operation);

    int result;
    switch(operation.at(operandPos)) {
        case '+':
            result = leftOperand + rightOperand;
        break;
        case '-':
            result = leftOperand - rightOperand;
        break;
        case '*':
            result = leftOperand * rightOperand;
        break;
        case '/':
            result = leftOperand / rightOperand;
        break;
        case 'm':
            result = leftOperand % rightOperand;
        break;
        case 'p':
            result = pow(leftOperand, rightOperand);
        break;
    }

    return result;
}

bool MainWindow::validID(int id)
{
    for(const auto& batch : batches) {
        for(const auto& process : batch->getProcesses()) {
            if(process->getId() == id) {
                QMessageBox::warning(this, tr("errorOperation, ID YA EXISTENTE"), tr("ID ya existente, por favor ingrese otra ID distinta"));
                ui->spnBx_ID->clear();
                errorID = true;
                return false;
            }
        }
    }
    errorID = false;
    return true;
}

void MainWindow::updateGlobalCounter(int i)
{
    ui->lcd_ContGlobal->display(i);
}

void MainWindow::updateTimeElapsed(int n)
{
    QTableWidgetItem *TT = new QTableWidgetItem(QString::number(n));
    ui->tblWdt_ProcesoEjec->setItem(0, TT_RP, TT);
}

void MainWindow::updateTimeLeft(int n) {
    QTableWidgetItem *TR = new QTableWidgetItem(QString::number(n));
    ui->tblWdt_ProcesoEjec->setItem(0, TR_RP, TR);
}

void MainWindow::updateTableFinish(Process *process) {
    ui->tblWdt_Terminados->setRowCount(rows);
    rows += 6;

    ui->tblWdt_Terminados->setVerticalHeaderItem(aux++, new QTableWidgetItem(tr("ID")));
    ui->tblWdt_Terminados->setVerticalHeaderItem(aux++, new QTableWidgetItem(tr("NOMBRE")));
    ui->tblWdt_Terminados->setVerticalHeaderItem(aux++, new QTableWidgetItem(tr("OPERACION")));
    ui->tblWdt_Terminados->setVerticalHeaderItem(aux++, new QTableWidgetItem(tr("RESULTADO")));
    ui->tblWdt_Terminados->setVerticalHeaderItem(aux++, new QTableWidgetItem(tr("TME")));
    ui->tblWdt_Terminados->setVerticalHeaderItem(aux++, new QTableWidgetItem(tr("LOTE")));
    ui->tblWdt_Terminados->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.

    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process->getId()));
    QTableWidgetItem *itemName = new QTableWidgetItem(process->getProgrammerName());
    QTableWidgetItem *itemOperation = new QTableWidgetItem(process->getOperation());
    QTableWidgetItem *itemResult = new QTableWidgetItem(QString::number(process->getResult()));
    QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(process->getTiempoMaximoEst()));
    QTableWidgetItem *itemLote = new QTableWidgetItem(QString::number(process->getNumBatch()));
    ui->tblWdt_Terminados->setItem(columns, ID_FP, itemID);
    ui->tblWdt_Terminados->setItem(columns, NOMBRE_FP, itemName);
    ui->tblWdt_Terminados->setItem(columns, OPERACION_FP, itemOperation);
    ui->tblWdt_Terminados->setItem(columns, RESULT_FP, itemResult);
    ui->tblWdt_Terminados->setItem(columns, TME_FP, itemTME);
    ui->tblWdt_Terminados->setItem(columns, LOTE_FP, itemLote);
    columns += 6;
}

void MainWindow::insertDataTableCurrentBatch()
{
    int row = 0;
    int totalProcesses = 0;

    for(const auto& batch : batches) {
        if(!batch->getIsAnalized()) {
            totalProcesses += batch->getSize();
        }
    }
    ui->tblWdt_LoteActual->setRowCount(totalProcesses);

    // NOT EFFICIENT, IT CHECKS EVERYTHING SINCE THE BEGGINING.
    for(const auto& batch : batches) {
        if(!batch->getIsAnalized()) {
            for(Process *process : batch->getProcesses()) {
                QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process->getId()));
                QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(process->getTiempoMaximoEst()));
                ui->tblWdt_LoteActual->setItem(row, ID, itemID);
                ui->tblWdt_LoteActual->setItem(row, TME, itemTME);
                ++row;

                threadProcessRunning->setProcess(process);
                threadTimeElapsed->setTME(process->getTiempoMaximoEst());
                threadTimeLeft->setTiemposRestantes(process->getTiempoMaximoEst());
                threadGlobalCounter->setTiemposEstimados(process->getTiempoMaximoEst());
            }
            batch->setIsAnalized(true);
        }
    }
    threadProcessRunning->start();
    threadTimeElapsed->start();
    threadTimeLeft->start();
    threadGlobalCounter->start();
}

void MainWindow::insertDataTableRunningProcess(Process* runningProcess) {
    int i;
    for(i = 0; i < 4; ++i) {
        QTableWidgetItem *ID = new QTableWidgetItem(QString::number(runningProcess->getId()));
        QTableWidgetItem *name = new QTableWidgetItem(runningProcess->getProgrammerName());
        QTableWidgetItem *operation = new QTableWidgetItem(runningProcess->getOperation());
        QTableWidgetItem *TME = new QTableWidgetItem(QString::number(runningProcess->getTiempoMaximoEst()));
        ui->tblWdt_ProcesoEjec->setItem(0, ID_RP, ID);
        ui->tblWdt_ProcesoEjec->setItem(0, NOMBRE_RP, name);
        ui->tblWdt_ProcesoEjec->setItem(0, OPERACION_RP, operation);
        ui->tblWdt_ProcesoEjec->setItem(0, TME_RP, TME);

        // If the process finished to be inserted.
        ++j;
        if(j == 4) {
            threadProcessRunning->finish = true;
            j = 0;
            updateTableFinish(runningProcess);
        }
    }
}

void MainWindow::sendData()
{
    if(!onlyOnce) {
        Batch *batch = new Batch;
        batches.push_back(batch);
        onlyOnce = true;
    }

    if(!firstTime) {
//        batches.clear(); // without deleting, you can validate any id from any batch.
        processRemaining = ui->spnBx_CantProcesos->value();
        firstTime = true;
    }

    if(batches.at(indexBatch)->getSize() == LIMITE_PROCESO) {
        Batch *batch = new Batch;
        batches.push_back(batch);
        ++indexBatch;
    }
    ui->spnBx_CantProcesos->setEnabled(false);

    insertProcess(indexBatch);

    if(!errorOperation and !errorID) {
        ui->lcd_ProcRestante->display(--processRemaining);
    }

    if(ui->lcd_ProcRestante->value() == 0) {
        ui->spnBx_CantProcesos->setValue(0);
        ui->spnBx_CantProcesos->setEnabled(true);
        firstTime = false;
    }

    ui->ldt_NombProgr->clear();
    ui->ldt_Operacion->clear();
    ui->spnBx_TME->setValue(0);
    ui->spnBx_ID->setValue(0);
    for(auto it = batches.begin(); it != batches.end(); ++it) {
        (*it)->showProccesses();
    }
    qDebug() << "indice del lote: " << indexBatch;
    qDebug() << "Cantidad de procesos en el lote: " << batches.at(indexBatch)->getSize();
}

void MainWindow::on_action_Procesar_Lote_triggered()
{
    if(!batches.empty()) {
        int processesInBatch = batches.at(indexBatch)->getSize();
        if(processesInBatch == LIMITE_PROCESO) {
            ui->ldt_NombProgr->setEnabled(false);
            ui->ldt_Operacion->setEnabled(false);
            ui->spnBx_ID->setEnabled(false);
            ui->spnBx_TME->setEnabled(false);
            ui->spnBx_CantProcesos->setEnabled(false);
            ui->btn_Enviar->setEnabled(false);

            threadProcessRunning->finish = false;
            insertDataTableCurrentBatch();

            ui->ldt_NombProgr->setEnabled(true);
            ui->ldt_Operacion->setEnabled(true);
            ui->spnBx_ID->setEnabled(true);
            ui->spnBx_TME->setEnabled(true);
            ui->spnBx_CantProcesos->setEnabled(true);
            ui->btn_Enviar->setEnabled(true);
        } else {
            QMessageBox::information(this, tr("Lote No Completado"), tr("Lote no completo, "
                                              "complete el lote para proceder"));
        }
    } else {
            QMessageBox::information(this, tr("Lotes Vacios"), tr("No hay procesos para analizar, "
                                                          "inserte procesos"));
    }
}
