#include "mainwindow.h"
#include "ui_mainwindow.h"

// TODO
// BUSCAR MANERA DE EVITAR MEMORY LEAKS
//
// NOTES:
//
// I used a lot of lists in each threads, I don't want to use only vectors to get only
// one element, find a better wa.
//
// VALIDAR OPERACION.
//
// Find a way to obtain a random number for names and operations, don't use rand.
// How to show a qmessagebox in a thread, this happens in reset function.

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , process(nullptr)
    , threadGlobalCounter(nullptr)
    , threadTimeElapsed(nullptr)
    , threadBatchCounter(nullptr)
    , threadCurrentTableBatch(nullptr)
    , threadTableRunning(nullptr)
    , threadTableFinish(nullptr)
    , errorOperation(false)
    , errorID(false)
    , firstTime(false)
    , onlyOnce(false)
    , randomData(false)
    , processInserted(0)
    , processRemaining(0)
    , batchNum(1)
    , indexBatch(0)
    , aux(0)
    , id(1)
    , globalCounter(0)
{
    ui->setupUi(this);

    // We do this to our mainwindow to get the keys pressed.
    this->setFocusPolicy(Qt::StrongFocus);

    threadGlobalCounter = new ThreadGlobalCounter;
    threadBatchCounter = new ThreadBatchCounter;
    threadTimeElapsed = new ThreadTImeElapsed;
    threadTimeLeft = new ThreadTImeLeft;

    threadCurrentTableBatch = new ThreadCurrentTableBatch;
    threadTableRunning = new ThreadTableRunning;
    threadTableFinish = new ThreadTableFinish;

    connect(threadGlobalCounter, &ThreadGlobalCounter::updateCounter, this, &MainWindow::updateGlobalCounter);
    connect(threadBatchCounter, &ThreadBatchCounter::updateBatchCounter, this, &MainWindow::updateBatchCounter);
    connect(threadTimeElapsed, &ThreadTImeElapsed::updateCounter, this, &MainWindow::updateTimeElapsed);
    connect(threadTimeLeft, &ThreadTImeLeft::updateCounter, this, &MainWindow::updateTimeLeft);
    connect(threadCurrentTableBatch, &ThreadCurrentTableBatch::updateTableCurrentBatch, this, &MainWindow::updateTableCurrentBatch);
    connect(threadTableRunning, &ThreadTableRunning::updateTableProcessRunning, this, &MainWindow::insertDataTableRunningProcess);
    connect(threadTableFinish, &ThreadTableFinish::updateTableFinish, this, &MainWindow::updateTableFinish);
    connect(threadTableFinish, &ThreadTableFinish::reset, this, &MainWindow::reset);

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

    for(auto& batch : batches) {
        delete batch;
        batch = nullptr;
    }

    delete threadGlobalCounter;
    delete threadBatchCounter;
    delete threadTimeElapsed;
    delete threadTimeLeft;

    delete threadCurrentTableBatch;
    delete threadTableRunning;
    delete threadTableFinish;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
        case Qt::Key_P:
            qDebug() << "PAUSE";
            threadGlobalCounter->pause();
            threadBatchCounter->pause();
            threadTimeElapsed->pause();
            threadTimeLeft->pause();
            threadCurrentTableBatch->pause();
//            threadGlobalCounter->pause();
//            threadTimeElapsed->pause();
//            threadTimeLeft->pause();
//            threadCurrentTableBatch->pause();
//            threadTableRunning->pause();
//            threadTableFinish->pause();

//            threadGlobalCounter->pause();
//            threadGlobalCounter->wait();
//            threadGlobalCounter->requestInterruption();
//            threadGlobalCounter->setStop(true);
//            threadBatchCounter->setStop(true);
//            threadTimeElapsed->setStop(true);
//            threadTimeLeft->setStop(true);
//            threadTables->terminate();
//            threadTables->setStop(true);
//            threadGlobalCounter->setStop(true);
//            threadGlobalCounter->setStop(true);
//            threadTables->stop = true;
//            threadTables->terminate();
//            threadTimeLeft->requestInterruption();
//            threadTimeElapsed->requestInterruption();
//            threadBatchCounter->requestInterruption();
//            threadGlobalCounter->requestInterruption();
        break;

        case Qt::Key_C:
            qDebug() << "CONTINUE";
            threadGlobalCounter->resume();
            threadBatchCounter->resume();
            threadTimeElapsed->resume();
            threadTimeLeft->resume();
            threadCurrentTableBatch->resume();
//            threadGlobalCounter->start();
//            threadBatchCounter->resume();
//            threadBatchCounter->start();
//            threadTableFinish->resume();
//            threadTableFinish->start();

//            threadBatchCounter->start();
//            threadGlobalCounter->start();
//            threadTimeLeft->start();
//            threadTimeElapsed->start();
//            threadTables->start();
        break;
    }
}

void MainWindow::removeSpace(std::string& operation) {
    std::string a;

    std::string toErase = " ";
    size_t pos = std::string::npos;
    while((pos = operation.find(toErase)) != std::string::npos) {
        operation.erase(pos, toErase.size());
    }
}

void MainWindow::runThreads()
{
    threadBatchCounter->currentBatchCounter = batches.size();

    // NOT EFFICIENT, IT CHECKS EVERYTHING SINCE THE BEGGINING.
    for(const auto& batch : batches) {
        batch->showProccesses();
        threadCurrentTableBatch->setBatch(batch);
        threadTableRunning->setBatch(batch);
        threadTableFinish->setBatch(batch);

        threadBatchCounter->setBatch(batch);

        for(Process *process : batch->getProcesses()) {
            threadTimeElapsed->setTME(process->getTiempoMaximoEst());
            threadTimeLeft->setTiemposRestantes(process->getTiempoMaximoEst());
            threadGlobalCounter->setTiemposEstimados(process->getTiempoMaximoEst());
        }
    }

    threadCurrentTableBatch->start();
    threadTableRunning->start();
    threadTableFinish->start();
    threadTimeElapsed->start();
    threadTimeLeft->start();
    threadGlobalCounter->start();
    threadBatchCounter->start();
}

void MainWindow::insertProcessByUser(int& index)
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
        process->setOperation(aux);
        process->setResult(doOperation(operation));

        int tiempoMaximoEst = ui->spnBx_TME->value();
        process->setTiempoMaximoEst(tiempoMaximoEst);

        int id = ui->spnBx_ID->value();

        if(validID(id)) {
            ids.push_back(id);
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

void MainWindow::insertProcessRandomly(int &index)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> randomTME(1,4);
    std::uniform_int_distribution<int> randomID(1,255);
    std::uniform_int_distribution<int> randomOperand(0, 500);

    QString names[10] = {"jose", "morales", "jimnez", "lupita", "Lucia",
                         "alberto", "pewdipew", "auron", "juana la loca", "andrea"};
    QString operators[6] = {"+", "-", "*", "/", "m", "p"};

    // Reserve how many batches I will need.
    int totalBatches = computeBatches(ui->spnBx_CantProcesos->value());
    for(int i = 0; i < totalBatches; ++i) {
        Batch *batch = new Batch;
        batches.push_back(batch);
    }

    for(int i = 0; i < ui->spnBx_CantProcesos->value(); ++i) {
        process = new Process;

        QString auxOperation;
        QString programmerName = names[rand() % 10]; // Get a random name.
        QString operand1 = QString::number(randomOperand(mt));
        QString operand2 = QString::number(randomOperand(mt));
        QString _operator = operators[rand() % 6];  // Get a random operator.

        auxOperation.append(operand1);
        auxOperation.append(_operator);
        auxOperation.append(operand2);
        std::string operation = auxOperation.toStdString();

        process->setProgrammerName(programmerName);
        process->setOperation(auxOperation);
        process->setResult(doOperation(operation));
        process->setTiempoMaximoEst(randomTME(mt));
        process->setId(id++);
        batches.at(index)->insertProcess(process);
        ui->lcd_LotesRestantes->display(batchNum);

        if(++processInserted == LIMITE_PROCESO) {
            ++indexBatch; // ? Is this ok?

            process->setNumBatch(batchNum);
            ui->lcd_LotesRestantes->display(batchNum++);
            processInserted = 0;
        } else {
            process->setNumBatch(batchNum);
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

int MainWindow::computeBatches(int numProcess)
{
    int batchesCount = 0;
    if(numProcess % 4 == 0) {
        batchesCount = numProcess / 4;
    } else {
        batchesCount = numProcess / 4;
        ++batchesCount;
    }
    return batchesCount;
}

long MainWindow::doOperation(std::string& operation) {
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
    for(const auto ID : ids) {
        if(id == ID) {
            QMessageBox::warning(this, tr("errorOperation, ID YA EXISTENTE"), tr("ID ya existente, por favor ingrese otra ID distinta"));
            ui->spnBx_ID->clear();
            errorID = true;
            return false;
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

void MainWindow::updateTableCurrentBatch(Batch *batch)
{
    ui->tblWdt_LoteActual->setRowCount(batch->getSize());

    int row = 0;
    for(const auto& process : batch->getProcesses()) {
        QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process->getId()));
        QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(process->getTiempoMaximoEst()));
        ui->tblWdt_LoteActual->setItem(row, ID, itemID);
        ui->tblWdt_LoteActual->setItem(row++, TME, itemTME);
    }
}

void MainWindow::insertDataTableCurrentBatch()
{
    ui->tblWdt_LoteActual->insertRow(ui->tblWdt_LoteActual->rowCount());
    int fila = ui->tblWdt_LoteActual->rowCount() - 1;

    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(ui->spnBx_ID->value()));
    QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(ui->spnBx_TME->value()));
    ui->tblWdt_LoteActual->setItem(fila, ID, itemID);
    ui->tblWdt_LoteActual->setItem(fila, TME, itemTME);
}

void MainWindow::insertDataTableRunningProcess(Process* runningProcess) {
    QTableWidgetItem *ID = new QTableWidgetItem(QString::number(runningProcess->getId()));
    QTableWidgetItem *name = new QTableWidgetItem(runningProcess->getProgrammerName());
    QTableWidgetItem *operation = new QTableWidgetItem(runningProcess->getOperation());
    QTableWidgetItem *TME = new QTableWidgetItem(QString::number(runningProcess->getTiempoMaximoEst()));
    ui->tblWdt_ProcesoEjec->setItem(0, ID_RP, ID);
    ui->tblWdt_ProcesoEjec->setItem(0, NOMBRE_RP, name);
    ui->tblWdt_ProcesoEjec->setItem(0, OPERACION_RP, operation);
    ui->tblWdt_ProcesoEjec->setItem(0, TME_RP, TME);
}

void MainWindow::reset()
{
    qDebug() << "reseteando";
    qDebug() << batches.size();

    for(auto& batch : batches) {
        delete batch;
    }
    batches.clear();

    errorOperation = false;
    errorID = false;
    firstTime = false;
    onlyOnce = false;
    randomData = false;
    processInserted = 0;
    processRemaining = 0;
    batchNum = 1;
    indexBatch = 0;
    ui->lcd_LotesRestantes->display(0);

////        threadGlobalCounter->setStop(false);
////        threadBatchCounter->setStop(false);
//        threadTimeElapsed->setStop(false);
//        threadTimeLeft->setStop(false);

    QMessageBox::information(this, tr("TERMINADO"), tr("Lotes analizados"));

    ui->tblWdt_ProcesoEjec->clearContents();
    ui->tblWdt_LoteActual->setRowCount(0);
}

void MainWindow::updateBatchCounter(int value)
{
    ui->lcd_LotesRestantes->display(value);
}

void MainWindow::sendData()
{
    if(!firstTime) {
        processRemaining = ui->spnBx_CantProcesos->value();
        firstTime = true;
    }

    if(!randomData) {
        if(!onlyOnce) {
            Batch *batch = new Batch;
            batches.push_back(batch);
            onlyOnce = true;
        }

        if(batches.at(indexBatch)->getSize() == LIMITE_PROCESO) {
            qDebug() << "dentro de seddata:";
            Batch *batch = new Batch;
            batches.push_back(batch);
            ++indexBatch;
        }

        ui->spnBx_CantProcesos->setEnabled(false);
        insertProcessByUser(indexBatch);

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
    } else {
        randomData = false;
        insertProcessRandomly(indexBatch);
    }

    qDebug() << "";
    for(auto it = batches.begin(); it != batches.end(); ++it) {
        (*it)->showProccesses();
    }
}

void MainWindow::on_action_Procesar_Lote_triggered()
{
    if(!batches.empty()) {
        ui->ldt_NombProgr->setEnabled(false);
        ui->ldt_Operacion->setEnabled(false);
        ui->spnBx_ID->setEnabled(false);
        ui->spnBx_TME->setEnabled(false);
        ui->spnBx_CantProcesos->setEnabled(false);
        ui->btn_Enviar->setEnabled(false);

        ui->tblWdt_LoteActual->setRowCount(0);
        runThreads();

        ui->ldt_NombProgr->setEnabled(true);
        ui->ldt_Operacion->setEnabled(true);
        ui->spnBx_ID->setEnabled(true);
        ui->spnBx_TME->setEnabled(true);
        ui->spnBx_CantProcesos->setEnabled(true);
        ui->btn_Enviar->setEnabled(true);
    } else {
        QMessageBox::information(this, tr("Lotes Vacios"), tr("No hay procesos para analizar, "
                                                          "inserte procesos"));
    }
}

void MainWindow::on_action_Procesar_Lote_con_Informacion_Aleatoria_triggered()
{
    if(ui->spnBx_CantProcesos->value() > 0) {
        randomData = true;
        sendData();
        runThreads();
    } else {
        QMessageBox::information(this, tr("Inserte Procesos"), tr("Inserte el numero de procesos para continuar"));
    }
}
