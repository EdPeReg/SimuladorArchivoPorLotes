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

- Memory leak qtablewidgetitem
- Use a list for batches and a deque for the processes.

*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , errorOperation(false)
    , errorID(false)
    , notFirstPauseTime(false)
    , onlyOnce(false)
    , randomData(false)
    , pauseRequired(false)
    , keyError(false)
    , IO_interruptionKey(false)
    , processInserted(0)
    , processRemaining(0)
    , batchNum(1)
    , indexBatch(0)
    , id(1)
    , globalCounter(0)
{
    ui->setupUi(this);

    // We do this to our mainwindow to get the keys pressed.
    this->setFocusPolicy(Qt::StrongFocus);

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
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(ui->spnBx_CantProcesos->value() == 0) {
        qDebug() << ui->spnBx_CantProcesos->value();
        QMessageBox::information(this, tr("Error"), tr("Inserte procesos para continuar"));
    } else {
        switch(event->key()) {
            // Pause.
            case Qt::Key_P:
                qDebug() << "PAUSE";
                ui->lnEdt_teclaPresionada->setText(tr("P"));
                ui->lnEdt_teclaPresionada->setAlignment(Qt::AlignCenter);
                pause();
            break;

            // Continue.
            case Qt::Key_C:
                qDebug() << "CONTINUE";
                ui->lnEdt_teclaPresionada->setText(tr("C"));
                ui->lnEdt_teclaPresionada->setAlignment(Qt::AlignCenter);
                resume();
            break;

            // Error.
            case Qt::Key_W:
                qDebug() << "ERROR";
                ui->lnEdt_teclaPresionada->setText(tr("W"));
                ui->lnEdt_teclaPresionada->setAlignment(Qt::AlignCenter);

                if(pauseRequired) {
                    QMessageBox::information(this, tr("Imposible continuar"), tr("Programa pausado, presiona C para continuar"));
                } else {
                    keyError = true;
                }
            break;

           // Interrupcion entrada salida.
           case Qt::Key_E:
                qDebug() << "INTERRUPCION ENTRADA SALIDA";
                ui->lnEdt_teclaPresionada->setText(tr("W"));
                ui->lnEdt_teclaPresionada->setAlignment(Qt::AlignCenter);

                if(pauseRequired) {
                    QMessageBox::information(this, tr("Imposible continuar"), tr("Programa pausado, presiona C para continuar"));
                } else {
                    ui->lnEdt_teclaPresionada->setText(tr("E"));
                    ui->lnEdt_teclaPresionada->setAlignment(Qt::AlignCenter);
                    IO_interruptionKey = true;
                }
            break;
        }
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

//void MainWindow::insertProcessByUser(int& index)
//{
//    process = new Process;
//    int rightOperand = 0;
//    QString programmerName = ui->ldt_NombProgr->text();
//    process->setProgrammerName(programmerName);

//    QString aux = ui->ldt_Operacion->text();
//    std::string operation = aux.toStdString();
//    removeSpace(operation);

//    int operatorPos = getOperatorPos(operation);
//    if(operation.at(operatorPos) == '/' or operation.at(operatorPos) == 'm') {
//        rightOperand = getRightOperand(operation);
//        if(rightOperand == 0) {
//            QMessageBox::warning(this, tr("errorOperation, OPERACION INVALIDA"), tr("Operacion invalida, ingrese una operacion valida"));
//            errorOperation = true;
//            ui->ldt_Operacion->clear();
//        } else {
//            errorOperation = false;
//        }
//    } else {
//        errorOperation = false;
//    }

//    if(!errorOperation) {
//        process->setOperation(aux);
//        process->setResult(doOperation(operation));

//        int tiempoMaximoEst = ui->spnBx_TME->value();
//        process->setTiempoMaximoEst(tiempoMaximoEst);

//        int id = ui->spnBx_ID->value();

//        if(validID(id)) {
//            ids.push_back(id);
//            process->setId(id);
//            batches.at(index)->insertProcess(process);
//            ui->lcd_LotesRestantes->display(batchNum);

//            if(++processInserted == LIMITE_PROCESO) {
//                process->setNumBatch(batchNum);
//                ui->lcd_LotesRestantes->display(batchNum++);
//                processInserted = 0;
//            } else {
//                process->setNumBatch(batchNum);
//            }
//            insertDataTableCurrentBatch();
//        }
//    }
//}

void MainWindow::insertProcessRandomly(int &index)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> randomTME(3,5);
    std::uniform_int_distribution<int> randomID(1,255);
    std::uniform_int_distribution<int> randomOperand(1, 500);

    QString names[10] = {"jose", "morales", "jimnez", "lupita", "Lucia",
                         "alberto", "pewdipew", "auron", "juana la loca", "andrea"};
    QString operators[6] = {"+", "-", "*", "/", "m", "p"};

    // Reserve how many batches I will need.
    int totalBatches = computeBatches(ui->spnBx_CantProcesos->value());

    for(int i = 0; i < totalBatches; ++i) {
        Batch batch;
        batches.push_back(batch);
    }

    for(int i = 0; i < ui->spnBx_CantProcesos->value(); ++i) {
        Process process;
        Batch batch;

        QString auxOperation;
        QString programmerName = names[rand() % 10]; // Get a random name.
        QString operand1 = QString::number(randomOperand(mt));
        QString operand2 = QString::number(randomOperand(mt));
        QString _operator = operators[rand() % 6];  // Get a random operator.

        auxOperation.append(operand1);
        auxOperation.append(_operator);
        auxOperation.append(operand2);
        std::string operation = auxOperation.toStdString();

        process.setProgrammerName(programmerName);
        process.setOperation(auxOperation);
        process.setResult(doOperation(operation));
        process.setTiempoMaximoEst(randomTME(mt));
        process.setId(id++);

        batches[index].insertProcess(process);
        ui->lcd_LotesRestantes->display(batchNum);

        if(++processInserted == LIMITE_PROCESO) {
            ++indexBatch; // ? Is this ok?

            process.setNumBatch(batchNum);
            ui->lcd_LotesRestantes->display(batchNum++);
            processInserted = 0;
        } else {
            process.setNumBatch(batchNum);
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
    pauseRequired = false;
    runWithRandomData();
}

void MainWindow::updateGlobalCounter(int value)
{
    ui->lcd_ContGlobal->display(value);
}

void MainWindow::updateTableFinish(Process process) {
    ui->tblWdt_Terminados->insertRow(ui->tblWdt_Terminados->rowCount());
    int fila = ui->tblWdt_Terminados->rowCount() - 1;

    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process.getId()));
    QTableWidgetItem *itemName = new QTableWidgetItem(process.getProgrammerName());
    QTableWidgetItem *itemOperation = new QTableWidgetItem(process.getOperation());
    QTableWidgetItem *itemResult = new QTableWidgetItem();

    // Insert an error state with red color. (this could be a function?).
    if(keyError) {
        int TT = process.getTT();
        QString TTstr = QString::number(TT);
        itemResult->setText(process.getEstado() + " TT " + TTstr);
        itemResult->setBackground(Qt::red);
    } else {
        itemResult->setText(QString::number(process.getResult()));
    }

    QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(process.getTiempoMaximoEst()));
    QTableWidgetItem *itemLote = new QTableWidgetItem(QString::number(process.getNumBatch()));
    ui->tblWdt_Terminados->setItem(fila, ID_FP, itemID);
    ui->tblWdt_Terminados->setItem(fila, NOMBRE_FP, itemName);
    ui->tblWdt_Terminados->setItem(fila, OPERACION_FP, itemOperation);
    ui->tblWdt_Terminados->setItem(fila, RESULT_FP, itemResult);
    ui->tblWdt_Terminados->setItem(fila, TME_FP, itemTME);
    ui->tblWdt_Terminados->setItem(fila, LOTE_FP, itemLote);
}

void MainWindow::runWithRandomData()
{
    int batchIndex = 0;
    int indexProcess = 0;
    int batchesRemaining = batches.size();
    updateBatchCounter(--batchesRemaining);

    // Iterate in our batches.
    while(!batches.empty()) {
        int row = 0;

        // Iterate in our processes.
        while(!batches.at(batchIndex).getProcesses().empty()) {
            Process process = batches.at(batchIndex).getProcesses().front(); // Get the process.
            int counterTimeElapsed = process.getTT();
            int counterTimeLeft = process.getTR();
            int indexTime = process.getIndexTime();

            updateTableCurrentBatch(row);

            // Iterate in our TME.
            while(indexTime < process.getTiempoMaximoEst()) {
                updateTT_TR_counters(counterTimeElapsed, counterTimeLeft);
                insertDataTableRunningProcess(process);
                updateGlobalCounter(++globalCounter);

                delay(1000);

                if(IO_interruptionKey) {
                    // To not insert more than three processes in our queue.
                    if(ui->tblWdt_LoteActual->rowCount() < LIMITE_PROCESO) {
                        // Update batch current table.
                        ui->tblWdt_LoteActual->insertRow(ui->tblWdt_LoteActual->rowCount());
                        int row = ui->tblWdt_LoteActual->rowCount() - 1;

                        QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process.getId()));
                        QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(process.getTiempoMaximoEst()));
                        QTableWidgetItem *itemTT= new QTableWidgetItem(QString::number(counterTimeElapsed));
                        ui->tblWdt_LoteActual->setItem(row, ID, itemID);
                        ui->tblWdt_LoteActual->setItem(row, TME, itemTME);
                        ui->tblWdt_LoteActual->setItem(row, 2, itemTT);

                        process.setTT(counterTimeElapsed);
                        process.setTR(counterTimeLeft);
                        process.setIndexTime(counterTimeElapsed);
                        batches[batchIndex].insertProcessFront(process);

                       // Simulate a queue.
                        batches[batchIndex].simulateQueue();
                    }
                    break;
                }

                if(pauseRequired) {
                    process.setTT(counterTimeElapsed);
                    process.setTR(counterTimeLeft);
                    process.setIndexTime(indexTime);
                    batches[batchIndex].insertProcessFront(process);
                    break;
                }

                if(keyError) {
                    process.setEstado("ERROR");
                    process.setTT(counterTimeElapsed);
                    batches[batchIndex].insertProcessFront(process);
                    break;
                }

                ++indexTime;
            }

            if(pauseRequired) break;

            if(!IO_interruptionKey) {
                updateTableFinish(process);
                process.setTT(counterTimeElapsed);
//                batches[batchIndex].insertProcessFront(process);
                batches[batchIndex].deleteProcess();
                indexTime = 0;
            }
            IO_interruptionKey = false;
            keyError = false;
        }

        if(pauseRequired) break;

        batches.removeFirst();
        updateBatchCounter(--batchesRemaining);

    }

    if(!pauseRequired) reset();
}

void MainWindow::updateTableCurrentBatch(int& row)
{
    int batchIndex = 0;

    int totalRows = batches.at(batchIndex).getProcesses().size();
    // Update table batch current.
    ui->tblWdt_LoteActual->setRowCount(totalRows--);
    for(int i = 0; i < batches.at(batchIndex).getProcesses().size(); ++i) {
        QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(batches.at(batchIndex).getProcesses().at(i).getId()));
        QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(batches.at(batchIndex).getProcesses().at(i).getTiempoMaximoEst()));
        QTableWidgetItem *itemTT= new QTableWidgetItem(QString::number(batches.at(batchIndex).getProcesses().at(i).getTT()));
        ui->tblWdt_LoteActual->setItem(row, ID, itemID);
        ui->tblWdt_LoteActual->setItem(row, TME, itemTME);
        ui->tblWdt_LoteActual->setItem(row++, TT, itemTT);
    }
    ui->tblWdt_LoteActual->removeRow(0);
}

void MainWindow::updateTT_TR_counters(int& counterTimeElapsed, int& counterTimeLeft)
{
    // Update Counters is this ok? using two times new.
    // Why not to use set text?
    QTableWidgetItem *TT = new QTableWidgetItem(QString::number(++counterTimeElapsed));
    QTableWidgetItem *TR = new QTableWidgetItem(QString::number(counterTimeLeft--));
    ui->tblWdt_ProcesoEjec->setItem(0, TT_RP, TT);
    ui->tblWdt_ProcesoEjec->setItem(0, TR_RP, TR);
}

//void MainWindow::insertDataTableCurrentBatch()
//{
//    ui->tblWdt_LoteActual->insertRow(ui->tblWdt_LoteActual->rowCount());
//    int fila = ui->tblWdt_LoteActual->rowCount() - 1;

//    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(ui->spnBx_ID->value()));
//    QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(ui->spnBx_TME->value()));
//    ui->tblWdt_LoteActual->setItem(fila, ID, itemID);
//    ui->tblWdt_LoteActual->setItem(fila, TME, itemTME);
//}

void MainWindow::insertDataTableRunningProcess(Process runningProcess) {
    QTableWidgetItem *ID = new QTableWidgetItem(QString::number(runningProcess.getId()));
    QTableWidgetItem *name = new QTableWidgetItem(runningProcess.getProgrammerName());
    QTableWidgetItem *operation = new QTableWidgetItem(runningProcess.getOperation());
    QTableWidgetItem *TME = new QTableWidgetItem(QString::number(runningProcess.getTiempoMaximoEst()));
    ui->tblWdt_ProcesoEjec->setItem(0, ID_RP, ID);
    ui->tblWdt_ProcesoEjec->setItem(0, NOMBRE_RP, name);
    ui->tblWdt_ProcesoEjec->setItem(0, OPERACION_RP, operation);
    ui->tblWdt_ProcesoEjec->setItem(0, TME_RP, TME);
}

void MainWindow::reset()
{
    qDebug() << "reseteando";
    qDebug() << batches.size();

    batches.clear();

    errorOperation = false;
    errorID = false;
    notFirstPauseTime = false;
    onlyOnce = false;
    randomData = false;
    keyError = false;
    pauseRequired = false;
    keyError = false;
    IO_interruptionKey = false;
    processInserted = 0;
    processRemaining = 0;
    batchNum = 1;
    indexBatch = 0;

    ui->tblWdt_ProcesoEjec->clearContents();
    ui->tblWdt_LoteActual->setRowCount(0);
    ui->lcd_LotesRestantes->display(0);
    ui->lnEdt_teclaPresionada->setText("");
    QMessageBox::information(this, tr("TERMINADO"), tr("Lotes analizados"));
}

void MainWindow::updateBatchCounter(int value)
{
    ui->lcd_LotesRestantes->display(value);
}

void MainWindow::sendData()
{
    if(!notFirstPauseTime) {
        processRemaining = ui->spnBx_CantProcesos->value();
        notFirstPauseTime = true;
    }

    if(!randomData) {
        if(!onlyOnce) {
            Batch batch;
            batches.push_back(batch);
            onlyOnce = true;
        }

        if(batches.at(indexBatch).getSize() == LIMITE_PROCESO) {
            qDebug() << "dentro de seddata:";
            Batch batch;
            batches.push_back(batch);
            ++indexBatch;
        }

        ui->spnBx_CantProcesos->setEnabled(false);
//        insertProcessByUser(indexBatch);

        if(!errorOperation and !errorID) {
            ui->lcd_ProcRestante->display(--processRemaining);
        }

        if(ui->lcd_ProcRestante->value() == 0) {
            ui->spnBx_CantProcesos->setValue(0);
            ui->spnBx_CantProcesos->setEnabled(true);
            notFirstPauseTime = false;
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
    for(const auto& batch : batches) {
        batch.showProccesses();
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

        // At the beggining, our initial value for TR is our TME, this will change.
        // After pressing P, E, W.
        for(auto& batch : batches) {
            batch.setInitialTR();
        }

        batchesCopy = batches;
        runWithRandomData();
    } else {
        QMessageBox::information(this, tr("Inserte Procesos"), tr("Inserte el numero de procesos para continuar"));
    }
}
