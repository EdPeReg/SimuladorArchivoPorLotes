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
    , pauseRequired(false)
    , keyError(false)
    , IO_interruptionKey(false)
    , startTTBCounter(false)
    , processInserted(0)
    , processesRemaining(0)
    , batchNum(1)
    , id(1)
    , globalCounter(0)
{
    ui->setupUi(this);

    // We do this to our mainwindow to get the keys pressed.
    this->setFocusPolicy(Qt::StrongFocus);

    ui->tblWdt_ProcListo->setColumnCount(3);
    ui->tblWdt_ProcListo->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    ui->tblWdt_ProcListo->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("TME")));
    ui->tblWdt_ProcListo->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("TT")));
    ui->tblWdt_ProcListo->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.

    ui->tblWgt_Bloqueados->setColumnCount(2);
    ui->tblWgt_Bloqueados->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    ui->tblWgt_Bloqueados->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("TTB")));
    ui->tblWgt_Bloqueados->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.

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

    ui->tblWdt_Terminados->setColumnCount(5);
    ui->tblWdt_Terminados->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("NOMBRE")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("OPERACION")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("RESULTADO")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("TME")));
    ui->tblWdt_Terminados->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.
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

void MainWindow::insertProcessRandomly()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> randomTME(5,8);
    std::uniform_int_distribution<int> randomID(1,255);
    std::uniform_int_distribution<int> randomOperand(1, 500);

    QString names[10] = {"jose", "morales", "jimnez", "lupita", "Lucia",
                         "alberto", "pewdipew", "auron", "juana la loca", "andrea"};
    QString operators[6] = {"+", "-", "*", "/", "m", "p"};

    for(int i = 0; i < ui->spnBx_CantProcesos->value(); ++i) {
        Process process;
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

        nuevos.push_back(process);
        ui->lcd_ProcRestantes->display(batchNum);
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

int MainWindow::computeQueues(int numProcess)
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

int MainWindow::getLeftOperand(const std::string& operation) {
    int operandPos = getOperandPos(operation);
    return stoi(operation.substr(0, operandPos));
}

int MainWindow::getRightOperand(const std::string& operation) {
    int operandPos = getOperandPos(operation);
    return stoi(operation.substr(operandPos + 1));
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
    ui->tblWdt_Terminados->setItem(fila, ID_FP, itemID);
    ui->tblWdt_Terminados->setItem(fila, NOMBRE_FP, itemName);
    ui->tblWdt_Terminados->setItem(fila, OPERACION_FP, itemOperation);
    ui->tblWdt_Terminados->setItem(fila, RESULT_FP, itemResult);
    ui->tblWdt_Terminados->setItem(fila, TME_FP, itemTME);
}

void MainWindow::runWithRandomData()
{

    while(true) {
        int counterTTB_p1 = 0;
        int counterTTB_p2 = 0;
        int counterTTB_p3 = 0;
        int counterTTB_p4 = 0;

        while(true) {
            Process process = listos.front(); // Get the first process.
            int row = 0;
            int counterTimeElapsed = process.getTT();
            int counterTimeLeft = process.getTR();
            int indexTime = process.getIndexTime();
            updateTableCurrentBatch(listos, row);

            // Iterate in our TME.
            while(indexTime < process.getTiempoMaximoEst()) {
                updateTT_TR_counters(counterTimeElapsed, counterTimeLeft);
                insertDataTableRunningProcess(process);
                updateGlobalCounter(++globalCounter);

                if(startTTBCounter) {
                    updateTTBCounter();
                }

                delay(1000);

                if(IO_interruptionKey) {
                    // To not insert more than four processes.
                    if(ui->tblWgt_Bloqueados->rowCount() < LIMITE_PROCESO) {
                        startTTBCounter = true;
                        process.setTT(counterTimeElapsed);
                        process.setTR(counterTimeLeft);
                        process.setIndexTime(++indexTime);

                        bloqueados.push_back(process);
                        updateBloqueadosTable(process);
                        listos.pop_front();
                    }
                    break;
                }

                if(pauseRequired) {
                    listos.at(0).setTT(counterTimeElapsed);
                    listos.at(0).setTR(counterTimeLeft);
                    listos.at(0).setIndexTime(++indexTime); // Increment to be in the right index.
                    break;
                }

                if(keyError) {
                    process.setEstado("ERROR");
                    process.setTT(counterTimeElapsed);
                    listos.pop_front();
                    listos.push_front(process);
                    break;
                }

                ++indexTime;
            }

            if(pauseRequired) break;

            if(!IO_interruptionKey) {
                terminados.push_back(process);
                listos.pop_front();

                if(!nuevos.empty()) {
                    listos.push_back(nuevos.front());
                    nuevos.pop_front();
                }

                updateTableFinish(process);
                process.setTT(counterTimeElapsed);

                // Update process counter.
                if(ui->lcd_ProcRestantes->value() != 0) {
                    ui->lcd_ProcRestantes->display(--processesRemaining);
                }

                indexTime = 0;
            }
            IO_interruptionKey = false;
            keyError = false;

            if(listos.empty()) {
                qDebug() << "YES";
                setNullProcess();
                break;
            }
        }

        while(!bloqueados.empty()) {
            qDebug() << "dentro de aqui";
            updateBloqueadosTable(bloqueados.at(0));
            updateTTBCounter();
            delay(1000);
//            qDebug() << "detroooooo";
        }

        // Out infinite loop.
        if(nuevos.empty() or pauseRequired) {
            break;
        }
    }

    if(!pauseRequired) reset();
}

void MainWindow::updateTableCurrentBatch(const std::deque<Process>& deque, int& row)
{
    int totalRows = deque.size();
    ui->tblWdt_ProcListo->setRowCount(totalRows--);
    for(const auto& process : deque) {
        QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process.getId()));
        QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(process.getTiempoMaximoEst()));
        QTableWidgetItem *itemTT= new QTableWidgetItem(QString::number(process.getTT()));
        ui->tblWdt_ProcListo->setItem(row, ID, itemID);
        ui->tblWdt_ProcListo->setItem(row, TME, itemTME);
        ui->tblWdt_ProcListo->setItem(row++, TT, itemTT);
    }
    ui->tblWdt_ProcListo->removeRow(0);
}

void MainWindow::insertLastTableListo(const Process &process)
{
    ui->tblWdt_ProcListo->insertRow(ui->tblWdt_ProcListo->rowCount());
    int fila = ui->tblWdt_ProcListo->rowCount() - 1;
    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process.getId()));
    QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(process.getTiempoMaximoEst()));
    QTableWidgetItem *itemTT= new QTableWidgetItem(QString::number(process.getTT()));
    ui->tblWdt_ProcListo->setItem(fila, ID, itemID);
    ui->tblWdt_ProcListo->setItem(fila, TME, itemTME);
    ui->tblWdt_ProcListo->setItem(fila++, TT, itemTT);
}

void MainWindow::updateTT_TR_counters(int& counterTimeElapsed, int& counterTimeLeft)
{
    QTableWidgetItem *TT = new QTableWidgetItem(QString::number(++counterTimeElapsed));
    QTableWidgetItem *TR = new QTableWidgetItem(QString::number(counterTimeLeft--));
    ui->tblWdt_ProcesoEjec->setItem(0, TT_RP, TT);
    ui->tblWdt_ProcesoEjec->setItem(0, TR_RP, TR);
}

void MainWindow::updateTTBCounter()
{
    // REVISAR.
    qDebug() << "bloqueados size: " << bloqueados.size();
    if(bloqueados.size() == 1) {
        if(bloqueados.at(0).getTTB() < LIMITE_TTB) {
            qDebug() << "dentro size = 1";
            int TTB_p1 = bloqueados.at(0).getTTB();
            QTableWidgetItem *itemTTB_p1 = new QTableWidgetItem(QString::number(++TTB_p1));
            ui->tblWgt_Bloqueados->setItem(0, TTB_BP, itemTTB_p1);
            bloqueados.at(0).setTTB(TTB_p1);
        } else {
            listos.push_back(bloqueados.at(0));
            bloqueados.pop_front();
            ui->tblWgt_Bloqueados->removeRow(0);
            insertLastTableListo(listos.back());
            startTTBCounter = false;
        }
    }

//    if(bloqueados.size() == 2) {
//        if(bloqueados.at(0).getTTB() < LIMITE_TTB) {
//            qDebug() << "dentro size = 2";
//            int TTB_p1 = bloqueados.at(0).getTTB();
//            int TTB_p2 = bloqueados.at(1).getTTB();
//            QTableWidgetItem *itemTTB_p1 = new QTableWidgetItem(QString::number(++TTB_p1));
//            QTableWidgetItem *itemTTB_p2 = new QTableWidgetItem(QString::number(++TTB_p2));
//            ui->tblWgt_Bloqueados->setItem(0, TTB_BP, itemTTB_p1);
//            ui->tblWgt_Bloqueados->setItem(1, TTB_BP, itemTTB_p2);
//            bloqueados.at(0).setTTB(TTB_p1);
//            bloqueados.at(1).setTTB(TTB_p2);
//        } else {
//            listos.push_back(bloqueados.at(0));
//            bloqueados.pop_front();
//            ui->tblWgt_Bloqueados->removeRow(0);
//            insertLastTableListo(listos.back());
//            startTTBCounter = false;
//        }

//        if(bloqueados.at(1).getTTB() < LIMITE_TTB) {
//            int TTB_p2 = bloqueados.at(1).getTTB();
//            QTableWidgetItem *itemTTB_p2 = new QTableWidgetItem(QString::number(++TTB_p2));
//            ui->tblWgt_Bloqueados->setItem(1, TTB_BP, itemTTB_p2);
//            bloqueados.at(1).setTTB(TTB_p2);
//        } else {
//            listos.push_back(bloqueados.at(1));
//            bloqueados.pop_front();
//            ui->tblWgt_Bloqueados->removeRow(1);
//            insertLastTableListo(listos.back());
//            startTTBCounter = false;
//        }
//    }

//    if(bloqueados.size() == 3) {
//        if(bloqueados.at(2).getTTB() < LIMITE_TTB) {
//            int TTB_p1 = bloqueados.at(0).getTTB();
//            int TTB_p2 = bloqueados.at(1).getTTB();
//            int TTB_p3 = bloqueados.at(2).getTTB();
//            QTableWidgetItem *itemTTB_p1 = new QTableWidgetItem(QString::number(++TTB_p1));
//            QTableWidgetItem *itemTTB_p2 = new QTableWidgetItem(QString::number(++TTB_p2));
//            QTableWidgetItem *itemTTB_p3 = new QTableWidgetItem(QString::number(++TTB_p3));
//            ui->tblWgt_Bloqueados->setItem(0, TTB_BP, itemTTB_p1);
//            ui->tblWgt_Bloqueados->setItem(1, TTB_BP, itemTTB_p2);
//            ui->tblWgt_Bloqueados->setItem(2, TTB_BP, itemTTB_p3);
//            bloqueados.at(0).setTTB(TTB_p1);
//            bloqueados.at(1).setTTB(TTB_p2);
//            bloqueados.at(2).setTTB(TTB_p3);
//        }
//    }

//    if(bloqueados.size() == 4) {
//        if(bloqueados.at(3).getTTB() < LIMITE_TTB) {
//            int TTB_p1 = bloqueados.at(0).getTTB();
//            int TTB_p2 = bloqueados.at(1).getTTB();
//            int TTB_p3 = bloqueados.at(2).getTTB();
//            int TTB_p4 = bloqueados.at(3).getTTB();
//            QTableWidgetItem *itemTTB_p1 = new QTableWidgetItem(QString::number(++TTB_p1));
//            QTableWidgetItem *itemTTB_p2 = new QTableWidgetItem(QString::number(++TTB_p2));
//            QTableWidgetItem *itemTTB_p3 = new QTableWidgetItem(QString::number(++TTB_p3));
//            QTableWidgetItem *itemTTB_p4 = new QTableWidgetItem(QString::number(++TTB_p4));
//            ui->tblWgt_Bloqueados->setItem(0, TTB_BP, itemTTB_p1);
//            ui->tblWgt_Bloqueados->setItem(1, TTB_BP, itemTTB_p2);
//            ui->tblWgt_Bloqueados->setItem(2, TTB_BP, itemTTB_p3);
//            ui->tblWgt_Bloqueados->setItem(3, TTB_BP, itemTTB_p4);
//            bloqueados.at(0).setTTB(TTB_p1);
//            bloqueados.at(1).setTTB(TTB_p2);
//            bloqueados.at(2).setTTB(TTB_p3);
//            bloqueados.at(3).setTTB(TTB_p4);
//        }
//    }
//    int TTB_p4 = bloqueados.at(3).getTTB();

//    QTableWidgetItem *itemTTB_p4 = new QTableWidgetItem(QString::number(++TTB_p4));
//    ui->tblWgt_Bloqueados->setItem(2, TTB_BP, itemTTB_p3);
//    ui->tblWgt_Bloqueados->setItem(3, TTB_BP, itemTTB_p4);
//    bloqueados.at(3).setTTB(TTB_p4);

//    for(auto& process : bloqueados) {
//        if(process.getTTB() < LIMITE_TTB) {
//            int TTB = process.getTTB();
//            QTableWidgetItem *item_TTB = new QTableWidgetItem(QString::number(++TTB));
//            ui->tblWgt_Bloqueados->setItem(0, TTB_BP, item_TTB);
//            bloqueados.at(0).setTTB(TTB);
//        } else {
//            listos.push_back(process);
//            bloqueados.pop_front();
//            ui->tblWgt_Bloqueados->removeRow(0);
//            insertLastTableListo(listos.back());
//            startTTBCounter = false;
//        }
//    }


//    if(bloqueados.at(0).getTTB() < LIMITE_TTB) {
//        int TTB = bloqueados.at(0).getTTB();
//        QTableWidgetItem *item_TTB = new QTableWidgetItem(QString::number(++TTB));
//        ui->tblWgt_Bloqueados->setItem(0, TTB_BP, item_TTB);
//        bloqueados.at(0).setTTB(TTB);
//    } else {
//        listos.push_back(bloqueados.at(0));
//        bloqueados.pop_front();
//        ui->tblWgt_Bloqueados->removeRow(0);
//        insertLastTableListo(listos.back());
//        startTTBCounter = false;
//    }
}

void MainWindow::updateBloqueadosTable(Process &process)
{
    ui->tblWgt_Bloqueados->insertRow(ui->tblWgt_Bloqueados->rowCount());
    int fila = ui->tblWgt_Bloqueados->rowCount() - 1;
    int TTB = process.getTTB();

    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process.getId()));
    ui->tblWgt_Bloqueados->setItem(fila, ID_BP, itemID);

}

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

    nuevos.clear();
    listos.clear();
    bloqueados.clear();

    errorOperation = false;
    errorID = false;
    notFirstPauseTime = false;
    keyError = false;
    pauseRequired = false;
    keyError = false;
    IO_interruptionKey = false;
    startTTBCounter= false;
    processInserted = 0;
    processesRemaining = 0;

    ui->tblWdt_ProcesoEjec->clearContents();
    ui->tblWdt_ProcListo->setRowCount(0);
    ui->lcd_ProcRestantes->display(0);
    ui->lnEdt_teclaPresionada->setText("");
    ui->spnBx_CantProcesos->setEnabled(true);
    ui->spnBx_CantProcesos->setValue(0);
    QMessageBox::information(this, tr("TERMINADO"), tr("Procesos analizados"));
}

void MainWindow::updateProcCounter(int value)
{
    ui->lcd_ProcRestantes->display(value);
}

void MainWindow::deleteProcessesNuevo()
{
    if(nuevos.size() <= 4) {
        while(!nuevos.empty()) {
            nuevos.pop_front();
        }
    } else {
        nuevos.erase(nuevos.begin(), nuevos.begin() + 4);
    }
}

void MainWindow::setInitialProcCounterValue()
{
    processesRemaining = ui->spnBx_CantProcesos->value();
    if(processesRemaining > LIMITE_PROCESO) {
        processesRemaining -= 4;
        ui->lcd_ProcRestantes->display(processesRemaining);
    } else {
        processesRemaining = 0;
        ui->lcd_ProcRestantes->display(processesRemaining);
    }
}

void MainWindow::setNullProcess()
{
    Process nullProcess;
    nullProcess.setId(-1);
    nullProcess.setProgrammerName("NULL");
    nullProcess.setOperation("NULL");
    int invalidNumber = -2;
    nullProcess.setTiempoMaximoEst(-1);
    nullProcess.setTR(invalidNumber);
    nullProcess.setTT(invalidNumber);
    insertDataTableRunningProcess(nullProcess);
    updateTT_TR_counters(invalidNumber, invalidNumber);
}

void MainWindow::aux()
{

}

std::deque<Process> MainWindow::slice(const std::deque<Process> &deque)
{
    std::deque<Process> listos;
    if(deque.size() <= 4) {
        for(auto& process : deque) {
            listos.push_back(process);
        }
    } else {
        // Just push the first four processes.
        for(size_t i = 0; i < deque.size(); ++i) {
            if(i < 4) {
                listos.push_back(deque.at(i));
            } else {
                break;
            }
        }

    }

    return listos;
}

void MainWindow::sendData()
{
    ui->spnBx_CantProcesos->setEnabled(false);
    insertProcessRandomly();

    qDebug() << "";
    int i = 1;
    for(const auto&  process : nuevos) {
        qDebug() << "\nProceso: " << i;
        qDebug() << "Name: " << process.getProgrammerName();
        qDebug() << "Operation: " << process.getOperation();
        qDebug() << "TME: " << process.getTiempoMaximoEst();
        qDebug() << "ESTADO: " << process.getEstado();
        qDebug() << "ID: " << process.getId();
        ++i;
    }
}

void MainWindow::on_action_Procesar_Procesos_triggered()
{
    if(ui->spnBx_CantProcesos->value() > 0) {
        sendData();

        // At the beggining, our initial value for TR is our TME, this will change.
        // After pressing P, E, W.
        for(auto& process : nuevos) {
            int aux = process.getTiempoMaximoEst();
            process.setTR(aux);
        }

        // Push the first processes to our listos deque.
        listos = slice(nuevos);
        deleteProcessesNuevo();

        setInitialProcCounterValue();
        runWithRandomData();
    } else {
        QMessageBox::information(this, tr("Inserte Procesos"), tr("Inserte el numero de procesos para continuar"));
    }
}
