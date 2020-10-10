#include "mainwindow.h"
#include "ui_mainwindow.h"

/*// TODO
// BUSCAR MANERA DE EVITAR MEMORY LEAKS
//
// NOTES:
//
// VALIDAR OPERACION.
//
// Find a way to obtain a random number for names and operations, don't use rand.
//
// WHen i click pause, continue and then pause, doesn't work.

- When the program is paused, the other keys shouldn't respond.

*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , process(nullptr)
    , errorOperation(false)
    , errorID(false)
    , firstTime(false)
    , onlyOnce(false)
    , randomData(false)
    , pauseRequired(false)
    , keyError(false)
    , processInserted(0)
    , processRemaining(0)
    , batchNum(1)
    , batchesRemaining(0)
    , indexBatch(0)
    , aux(0)
    , id(1)
    , globalCounter(0)
{
    ui->setupUi(this);

    // We do this to our mainwindow to get the keys pressed.
    this->setFocusPolicy(Qt::StrongFocus);

    threadGlobalCounter = new ThreadGlobalCounter;

//    connect(threadGlobalCounter, &ThreadGlobalCounter::updateCounter, this, &MainWindow::updateGlobalCounter);

    ui->tblWdt_LoteActual->setColumnCount(3);
    ui->tblWdt_LoteActual->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    ui->tblWdt_LoteActual->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("TME")));
    ui->tblWdt_LoteActual->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("TT")));
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
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
        // Pause.
        case Qt::Key_P:
            qDebug() << "PAUSE";
//            threadGlobalCounter->pause();
            pause();
        break;

        // Continue.
        case Qt::Key_C:
            qDebug() << "CONTINUE";
//            threadGlobalCounter->resume();
            resume();
        break;

        // Error.
        case Qt::Key_W:
            qDebug() << "ERROR";
            keyError = true;
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

void MainWindow::runGlobalCounterThread()
{
//    // NOT EFFICIENT, IT CHECKS EVERYTHING SINCE THE BEGGINING.
    for(const auto& batch : batches) {
        batch->showProccesses();
        for(Process *process : batch->getProcesses()) {
            threadGlobalCounter->setTiemposEstimados(process->getTiempoMaximoEst());
        }
    }

//    threadGlobalCounter->start();
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

// THX dvntehn00bz,
// https://stackoverflow.com/questions/3752742/how-do-i-create-a-pause-wait-function-using-qt
void MainWindow::delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

void MainWindow::pause()
{
    pauseRequired = true;
}

void MainWindow::resume()
{
    updateTableCurrentBatch();
}

void MainWindow::updateGlobalCounter(int value)
{
    ui->lcd_ContGlobal->display(value);
}

// REPEATED  CODE, BAD!! >:V
void MainWindow::updateTimeCounters(Batch *batch)
{
    QList<Process *> processes = batch->getProcesses();

    int indexProcess = 0;
    // Iterate in my process list.
    while(indexProcess < processes.size()) {
        int counterTimeElapsed = 0;
        int counterTimeLeft = processes.at(indexProcess)->getTiempoMaximoEst();

        int i = 0;
        // Iterate in our TME.
        while(i < processes.at(indexProcess)->getTiempoMaximoEst()) {
            QTableWidgetItem *TT = new QTableWidgetItem(QString::number(++counterTimeElapsed));
            QTableWidgetItem *TR = new QTableWidgetItem(QString::number(counterTimeLeft--));
            ui->tblWdt_ProcesoEjec->setItem(0, TT_RP, TT);
            ui->tblWdt_ProcesoEjec->setItem(0, TR_RP, TR);

            insertDataTableRunningProcess(processes.at(indexProcess));
            updateGlobalCounter(++globalCounter);
            delay(1000);

            // There is an error, go to the next process and update the table with that process.
//            if(keyError) { // CHECK
//                saveState.TME = processes.at(indexProcess)->getTiempoMaximoEst();
//                threadGlobalCounter->setTT(counterTimeElapsed); // ?
//                threadGlobalCounter->setKeyError(keyError); // ?

//                processes.at(indexProcess)->setEstado("ERROR");
//                updateTableFinish(processes.at(indexProcess));
//                keyError = false;
//                break;
//            }

//			if(pauseRequired) {
//				int aux = counterTimeLeft; // We have a copy because we don't want to really change the var.
//				saveState.counterTimeLeft = ++aux;
//				saveState.counterTimeElapsed = counterTimeElapsed;

//				qDebug() << "counter time elapsed: " << saveState.counterTimeElapsed;
//				qDebug() << "counter time left: " << saveState.counterTimeLeft;
//				break;
//			}

            ++i;
        }

//		if(pauseRequired) {
//			saveState.processTableFinish = processes.at(indexProcess);
//			saveState.indexProcess = indexProcess;
//			qDebug() << "Process use for table finish saved id: " << saveState.processTableFinish->getId();
//			qDebug() << "We stoped at this index process: " << saveState.indexProcess;
//			break;
//		}

        // Just update the table if the process doesn't have any error.
        if(processes.at(indexProcess)->getEstado() != "ERROR") {
            updateTableFinish(processes.at(indexProcess));
        }
        ++indexProcess;
    }


//    int indexProcess = 0;

//    if(!pauseRequired) {
//        for(auto& process : batch->getProcesses()) {
//            int counterTimeElapsed = 0;
//            int counterTimeLeft = process->getTiempoMaximoEst();
//            for(int i = 0; i < process->getTiempoMaximoEst(); ++i) {
//                QTableWidgetItem *TT = new QTableWidgetItem(QString::number(++counterTimeElapsed));
//                QTableWidgetItem *TR = new QTableWidgetItem(QString::number(counterTimeLeft--));
//                ui->tblWdt_ProcesoEjec->setItem(0, TT_RP, TT);
//                ui->tblWdt_ProcesoEjec->setItem(0, TR_RP, TR);

//                insertDataTableRunningProcess(process);
//                delay(1000);

//                // There is an error, go to the next process and update the table with that process.
//                if(keyError) {
//                    saveState.TME = process->getTiempoMaximoEst();
//                    threadGlobalCounter->setTT(counterTimeElapsed);
//                    threadGlobalCounter->setKeyError(keyError);

//                    process->setEstado("ERROR");
//                    updateTableFinish(process);
//                    keyError = false;
////                    threadGlobalCounter->setKeyError(keyError);
//                    break;
//                }

//                if(pauseRequired) {
//                    int aux = counterTimeLeft; // We have a copy because we don't want to really change the var.
//                    saveState.counterTimeLeft = ++aux;
//                    saveState.counterTimeElapsed = counterTimeElapsed;

//                    qDebug() << "counter time elapsed: " << saveState.counterTimeElapsed;
//                    qDebug() << "counter time left: " << saveState.counterTimeLeft;
//                    break;
//                }
//            }

//            if(pauseRequired) {
//                saveState.processTableFinish = process;
//                saveState.indexProcess = indexProcess;
//                qDebug() << "Process use for table finish saved id: " << saveState.processTableFinish->getId();
//                qDebug() << "We stoped at this index process: " << saveState.indexProcess;
//                break;
//            }

//            // Just update the table if the process doesn't have any error.
//            if(process->getEstado() != "ERROR") {
//                updateTableFinish(process);
//            }
//            ++indexProcess;
//        }
//    } else {
//        pauseRequired = false;
//        QList<Process *> processes = batch->getProcesses();
//        int timeElapsed = saveState.counterTimeElapsed;
//        int indexProcess = saveState.indexProcess;

//        while(indexProcess < processes.size()) {
//            qDebug() << "i inside the else: " << indexProcess;
//            qDebug() << "processes.size: " << processes.size();
//            int counterTimeElapsed = saveState.counterTimeElapsed;
//            int counterTimeLeft = saveState.counterTimeLeft;

//            // We start from the Time elapsed, because that was the last value that we had.
//            // before the pause.
//            while(timeElapsed < processes.at(indexProcess)->getTiempoMaximoEst()) {
//                qDebug() << "time elapsed inside the for: " << timeElapsed;
//                qDebug() << "process at(i) TME: " << processes.at(indexProcess)->getTiempoMaximoEst();
//                qDebug() << "counter time elapse: " << counterTimeElapsed;
//                qDebug() << "counter time left: " << counterTimeLeft;

//                QTableWidgetItem *TT = new QTableWidgetItem(QString::number(++counterTimeElapsed));
//                QTableWidgetItem *TR = new QTableWidgetItem(QString::number(--counterTimeLeft));
//                ui->tblWdt_ProcesoEjec->setItem(0, TT_RP, TT);
//                ui->tblWdt_ProcesoEjec->setItem(0, TR_RP, TR);

//                qDebug() << "i valu before insert data table running process: " << indexProcess;
//                insertDataTableRunningProcess(processes.at(indexProcess));

//                // Somehow, after the previous line, our lcd batch counter shows our initial
//                // batches couter, why? only god knos, so let's display its correct value.
//                ui->lcd_LotesRestantes->display(saveState.batchCounter);
//                delay(1000);

//                // There is an error, go to the next process and update the table with that process.
//                if(keyError) {
//                    processes.at(indexProcess)->setEstado("ERROR");
//                    updateTableFinish(processes.at(indexProcess));
//                    keyError = false;
//                    break;
//                }

//                if(pauseRequired) {
//                    // CHECK THIS.
//                    int aux = counterTimeLeft; // We have a copy because we don't want to really change the var.
//                    saveState.counterTimeLeft = ++aux;
//                    saveState.counterTimeElapsed = counterTimeElapsed;

//                    qDebug() << "counter time elapsed: " << saveState.counterTimeElapsed;
//                    qDebug() << "counter time left: " << saveState.counterTimeLeft;
//                    break;
//                }
//                ++timeElapsed;
//            }

//            // Just update the table if the process doesn't have any error.
//            if(processes.at(indexProcess)->getEstado() != "ERROR") {
//                updateTableFinish(processes.at(indexProcess));
//            }

//            // To avoid out of range after incrementing i.
//            if(++indexProcess == processes.size()) {
//                break;
//            }

//            // Reset our values, to not have the previous values.
//            saveState.counterTimeElapsed = 0;
//            saveState.counterTimeLeft = processes.at(indexProcess)->getTiempoMaximoEst() + 1;

//            // We want to start from the beggining in our index process and our time elapsed.
//            timeElapsed = 0;
//        }
//    }
}

void MainWindow::updateTableFinish(Process *process) {
    ui->tblWdt_Terminados->insertRow(ui->tblWdt_Terminados->rowCount());
    int fila = ui->tblWdt_Terminados->rowCount() - 1;

    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process->getId()));
    QTableWidgetItem *itemName = new QTableWidgetItem(process->getProgrammerName());
    QTableWidgetItem *itemOperation = new QTableWidgetItem(process->getOperation());
    QTableWidgetItem *itemResult = new QTableWidgetItem();

//    if(keyError) {
//        itemResult->setText(process->getEstado());
//        itemResult->setBackground(Qt::red);
//    } else {
        itemResult->setText(QString::number(process->getResult()));
//    }

    QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(process->getTiempoMaximoEst()));
    QTableWidgetItem *itemLote = new QTableWidgetItem(QString::number(process->getNumBatch()));
    ui->tblWdt_Terminados->setItem(fila, ID_FP, itemID);
    ui->tblWdt_Terminados->setItem(fila, NOMBRE_FP, itemName);
    ui->tblWdt_Terminados->setItem(fila, OPERACION_FP, itemOperation);
    ui->tblWdt_Terminados->setItem(fila, RESULT_FP, itemResult);
    ui->tblWdt_Terminados->setItem(fila, TME_FP, itemTME);
    ui->tblWdt_Terminados->setItem(fila, LOTE_FP, itemLote);
}

// REPEATED CODE: BAD! >:v
void MainWindow::updateTableCurrentBatch()
{
    batchesRemaining = batches.size();
    ui->lcd_LotesRestantes->display(--batchesRemaining);

    int batchIndex = 0;
    // Iterate in our batches.
    while(batchIndex < batches.size()) {
        int row = 0;
        ui->tblWdt_LoteActual->setRowCount(batches.at(batchIndex)->getSize());

        QList<Process *> processes = batches.at(batchIndex)->getProcesses();
        int processIndex = 0;
        while(processIndex < processes.size()) {
            QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(processes.at(processIndex)->getId()));
            QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(processes.at(processIndex)->getTiempoMaximoEst()));
            QTableWidgetItem *itemTT= new QTableWidgetItem("0");
            ui->tblWdt_LoteActual->setItem(row, ID, itemID);
            ui->tblWdt_LoteActual->setItem(row, TME, itemTME);
            ui->tblWdt_LoteActual->setItem(row++, TT, itemTT);
            ++processIndex;
        }

        updateTimeCounters(batches.at(batchIndex));
//        if(pauseRequired) {
//            saveState.batchCounter = batchesRemaining;
//            saveState.batchIndex = batchIndex;
//            saveState.batchCurrentBatch = batches.at(batchIndex);

//            qDebug() << "paused at this index batch: " << saveState.batchIndex;
//            qDebug() << "paused at batch counter: " << saveState.batchCounter;
//            saveState.batchCurrentBatch->showProccesses();
//            break;
//        }

        // If we don't do this, our batch counter will be a negative number.
        if(batchIndex != batches.size() - 1) {
            updateBatchCounter(--batchesRemaining);
            saveState.batchCounter = batchesRemaining;
        }

        ++batchIndex;
    }

    if(!pauseRequired) reset();


/*    batchesRemaining = batches.size();
    ui->lcd_LotesRestantes->display(--batchesRemaining);

    int batchIndex = 0;
    if(!pauseRequired) {
        for(const auto& batch : batches) {
            int row = 0;
            ui->tblWdt_LoteActual->setRowCount(batch->getSize());
            for(const auto& process : batch->getProcesses()) {
                QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process->getId()));
                QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(process->getTiempoMaximoEst()));
                QTableWidgetItem *itemTT= new QTableWidgetItem("0");
                ui->tblWdt_LoteActual->setItem(row, ID, itemID);
                ui->tblWdt_LoteActual->setItem(row, TME, itemTME);
                ui->tblWdt_LoteActual->setItem(row++, TT, itemTT);
            }

            updateTimeCounters(batch);
            if(pauseRequired) {
                saveState.batchCounter = batchesRemaining;
                saveState.batchIndex = batchIndex;
                saveState.batchCurrentBatch = batch;

                qDebug() << "paused at this index batch: " << saveState.batchIndex;
                qDebug() << "paused at batch counter: " << saveState.batchCounter;
                saveState.batchCurrentBatch->showProccesses();
                break;
            }
            // If we don't do this, our batch counter will be a negative number.
            if(batchIndex != batches.size() - 1) {
                updateBatchCounter(--batchesRemaining);
                saveState.batchCounter = batchesRemaining;
            }
            ++batchIndex;
        }
    } else {
        // Update our batches remaining, because at this point, our batches remaining
        // arent updated. because break.
        batchesRemaining = saveState.batchCounter;

        for(int i = saveState.batchIndex; i < batches.size(); ++i) {
            int row = 0;
            ui->tblWdt_LoteActual->setRowCount(batches.at(i)->getSize());
            QList<Process *> processes = batches.at(i)->getProcesses();

            for(int j = 0; j < processes.size(); ++j) {
                QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(processes.at(j)->getId()));
                QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(processes.at(j)->getTiempoMaximoEst()));
                ui->tblWdt_LoteActual->setItem(row, ID, itemID);
                ui->tblWdt_LoteActual->setItem(row++, TME, itemTME);
            }

            updateTimeCounters(batches.at(i));

            if(pauseRequired) {
                saveState.batchCounter = batchesRemaining;
                saveState.batchCurrentBatch = batches.at(i);

                qDebug() << "paused at batch counter: " << saveState.batchCounter;
                saveState.batchCurrentBatch->showProccesses();
                break;
            }
            // If we don't do this, our batch counter will be a negative number.
            qDebug() << "bach index: " << batchIndex;
            if(batchIndex != batches.size() - 1) {
                qDebug() << "dentro del batchindex !- batches.size";
                updateBatchCounter(--batchesRemaining);
                saveState.batchCounter = batchesRemaining;
            }
            ++batchIndex;
        }
    }

    if(!pauseRequired) reset()*/;
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
    keyError = false;
    processInserted = 0;
    processRemaining = 0;
    batchNum = 1;
    indexBatch = 0;
    ui->lcd_LotesRestantes->display(0);

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
        runGlobalCounterThread();

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
        runGlobalCounterThread();
        updateTableCurrentBatch();
    } else {
        QMessageBox::information(this, tr("Inserte Procesos"), tr("Inserte el numero de procesos para continuar"));
    }
}
