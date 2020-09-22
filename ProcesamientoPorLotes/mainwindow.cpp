#include "mainwindow.h"
#include "ui_mainwindow.h"

// TODO
// BUSCAR MANERA DE EVITAR MEMORY LEAKS
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
    , process(nullptr)
    , threadGlobalCounter(nullptr)
    , threadProcessRunning(nullptr)
    , threadTimeElapsed(nullptr)
    , threadBatchCounter(nullptr)
    , errorOperation(false)
    , errorID(false)
    , firstTime(false)
    , onlyOnce(false)
    , processInserted(0)
    , processRemaining(0)
    , batchNum(1)
    , indexBatch(0)
    , aux(0)
{
    ui->setupUi(this);

    threadGlobalCounter = new ThreadGlobalCounter;
    threadBatchCounter = new ThreadBatchCounter;
    threadProcessRunning = new ThreadProcessRunning;
    threadTimeElapsed = new ThreadTImeElapsed;
    threadTimeLeft = new ThreadTImeLeft;

    connect(threadGlobalCounter, &ThreadGlobalCounter::updateCounter, this, &MainWindow::updateGlobalCounter);
    connect(threadBatchCounter, &ThreadBatchCounter::updateBatchCounter, this, &MainWindow::updateBatchCounter);
    connect(threadProcessRunning, &ThreadProcessRunning::updateTable, this, &MainWindow::insertDataTableRunningProcess);
    connect(threadProcessRunning, &ThreadProcessRunning::reset, this, &MainWindow::reset);
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

    ui->tblWdt_Terminados->setColumnCount(6);
    ui->tblWdt_Terminados->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("NOMBRE")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("OPERACION")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("RESULTADO")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("TME")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("LOTE")));
    ui->tblWdt_Terminados->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.

    connect(ui->btn_Enviar, &QPushButton::clicked, this, &MainWindow::sendData);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete threadGlobalCounter;
    delete threadBatchCounter;
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
    process = new Process;
    int rightOperand = 0;
    QString programmerName = ui->ldt_NombProgr->text();
    process->setProgrammerName(programmerName);

    QString aux = ui->ldt_Operacion->text();
    std::string operation = aux.toStdString();
    removeSpace(operation);

    int operatorPos = getOperatorPos(operation);
    if(operation.at(operatorPos) == '/' or operation.at(operatorPos) == 'm') {
        rightOperand = getRightOperand(operation);
        if(rightOperand == 0) {
            QMessageBox::warning(this, tr("errorOperation, OPERACION INVALIDA"), tr("Operacion invalida, ingrese una operacion valida"));
            errorOperation = true;
            ui->ldt_Operacion->clear();
        } else {
            errorOperation = false;
        }
    } else {
        errorOperation = false;
    }

    if(!errorOperation) {
        qDebug() << "dentro de !error operation";
        process->setOperation(aux);
        process->setResult(doOperation(operation));

        int tiempoMaximoEst = ui->spnBx_TME->value();
        process->setTiempoMaximoEst(tiempoMaximoEst);

        int id = ui->spnBx_ID->value();

        if(validID(id)) {
            process->setId(id);
            batches.at(index)->insertProcess(process);
            ui->lcd_LotesRestantes->display(batchNum);

            if(++processInserted == LIMITE_PROCESO) {
                process->setNumBatch(batchNum);
                ui->lcd_LotesRestantes->display(batchNum++);
                processInserted = 0;
            } else {
                process->setNumBatch(batchNum);
            }
            insertDataTableCurrentBatch();
            qDebug() << " ";
            qDebug() << "Indice antes de insertar: " << index;
            qDebug() << "LOtes totales antes de insertar: " << batches.size();
        }
    }
}

int MainWindow::getOperatorPos(const std::string& operation) {
    size_t posOperator = operation.find_first_of("/m");
    if(posOperator != std::string::npos) {
        return posOperator;
    }
    return 0;
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

void MainWindow::updateGlobalCounter(int value)
{
    ui->lcd_ContGlobal->display(value);
}

void MainWindow::updateTimeElapsed(int value)
{
    QTableWidgetItem *TT = new QTableWidgetItem(QString::number(value));
    ui->tblWdt_ProcesoEjec->setItem(0, TT_RP, TT);
}

void MainWindow::updateTimeLeft(int value) {
    QTableWidgetItem *TR = new QTableWidgetItem(QString::number(value));
    ui->tblWdt_ProcesoEjec->setItem(0, TR_RP, TR);
}

void MainWindow::updateTableFinish(Process *process) {

    ui->tblWdt_Terminados->insertRow(ui->tblWdt_Terminados->rowCount());
    int fila = ui->tblWdt_Terminados->rowCount() - 1;

    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process->getId()));
    QTableWidgetItem *itemName = new QTableWidgetItem(process->getProgrammerName());
    QTableWidgetItem *itemOperation = new QTableWidgetItem(process->getOperation());
    QTableWidgetItem *itemResult = new QTableWidgetItem(QString::number(process->getResult()));
    QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(process->getTiempoMaximoEst()));
    QTableWidgetItem *itemLote = new QTableWidgetItem(QString::number(process->getNumBatch()));
    ui->tblWdt_Terminados->setItem(fila, ID_FP, itemID);
    ui->tblWdt_Terminados->setItem(fila, NOMBRE_FP, itemName);
    ui->tblWdt_Terminados->setItem(fila, OPERACION_FP, itemOperation);
    ui->tblWdt_Terminados->setItem(fila, RESULT_FP, itemResult);
    ui->tblWdt_Terminados->setItem(fila, TME_FP, itemTME);
    ui->tblWdt_Terminados->setItem(fila, LOTE_FP, itemLote);
}

void MainWindow::insertDataTableCurrentBatch()
{
    ui->tblWdt_LoteActual->insertRow(ui->tblWdt_LoteActual->rowCount());
    int fila = ui->tblWdt_LoteActual->rowCount() - 1;

    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(ui->spnBx_ID->value()));
    QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(ui->spnBx_TME->value()));
    ui->tblWdt_LoteActual->setItem(fila, ID, itemID);
    ui->tblWdt_LoteActual->setItem(fila, TME, itemTME);

//    int row = 0;
//    int totalProcesses = 0;

//    for(const auto& batch : batches) {
//        if(!batch->isAnalized()) {
//            totalProcesses += batch->getSize();
//        }
//    }
//    ui->tblWdt_LoteActual->setRowCount(totalProcesses);
//    threadBatchCounter->currentBatchCounter = batchNum;

    // NOT EFFICIENT, IT CHECKS EVERYTHING SINCE THE BEGGINING.
//    for(const auto& batch : batches) {
//        threadBatchCounter->setBatch(batch);
//        if(!batch->isAnalized()) {
//            for(Process *process : batch->getProcesses()) {
//                QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process->getId()));
//                QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(process->getTiempoMaximoEst()));
//                ui->tblWdt_LoteActual->setItem(row, ID, itemID);
//                ui->tblWdt_LoteActual->setItem(row, TME, itemTME);
//                ++row;

//                threadProcessRunning->setProcess(process);
//                threadTimeElapsed->setTME(process->getTiempoMaximoEst());
//                threadTimeLeft->setTiemposRestantes(process->getTiempoMaximoEst());
//                threadGlobalCounter->setTiemposEstimados(process->getTiempoMaximoEst());
//            }
//            batch->setIsAnalized(true);
//        }
//    }
//    threadProcessRunning->start();
//    threadTimeElapsed->start();
//    threadTimeLeft->start();
//    threadGlobalCounter->start();
//    threadBatchCounter->start();
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

void MainWindow::reset()
{
    qDebug() << "reseteando";
    for(auto& batch : batches) {
        delete batch;
        batch = nullptr;
    }
    batches.clear();

    errorOperation = false;
    errorID = false;
    firstTime = false;
    onlyOnce = false;
    processInserted = 0;
    processRemaining = 0;
    batchNum = 1;
    indexBatch = 0;
    j = 0;
    ui->lcd_LotesRestantes->display(0);

        QMessageBox::information(this, tr("TERMINADO"), tr("Lotes analizados"));

    ui->tblWdt_LoteActual->clearContents();
    ui->tblWdt_ProcesoEjec->clearContents();
}

void MainWindow::updateBatchCounter(int value)
{
    ui->lcd_LotesRestantes->display(value);
}

void MainWindow::sendData()
{
    if(!onlyOnce) {
        Batch *batch = new Batch;
        batches.push_back(batch);
        onlyOnce = true;
    }

    if(!firstTime) {
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
//    insertDataTableCurrentBatch();

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
//            insertDataTableCurrentBatch();

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
