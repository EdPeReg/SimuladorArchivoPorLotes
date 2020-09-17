#include "mainwindow.h"
#include "ui_mainwindow.h"

// TODO
// BUSCAR MANERA DE EVITAR MEMORY LEAKS
// VALIDAR MQUE EL 0 FUNCIONE EN LAS DEMAS OPERACIONES .

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , errorOperation(false)
    , errorID(false)
    , firstTime(false)
    , onlyOnce(false)
    , processInserted(0)
    , processRemaining(0)
    , batchesCount(0)
    , batchNum(1)
    , indexBatch(0)
    , globalCounter(0)
{
    ui->setupUi(this);

    threadGlobalCounter = new ThreadGlobalCounter;

    connect(threadGlobalCounter, &ThreadGlobalCounter::updateGlobalCounter, [&]() {
        globalCounter = 0;
        for(const auto& batch : batches) {
            for(const auto& process : batch->getProcesses()) {
                int aux = 1;
                for(int i = 0; i < process->getTiempoMaximoEst(); ++i) {
                    qDebug() << aux++;
                    ui->lcd_ContGlobal->display(++globalCounter);
                }
            }
        }
    });

    ui->tblWdt_LoteActual->setColumnCount(2);
    ui->tblWdt_LoteActual->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    ui->tblWdt_LoteActual->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("TME")));
    ui->tblWdt_LoteActual->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.

    ui->tblWdt_ProcesoEjec->setColumnCount(2);
    ui->tblWdt_ProcesoEjec->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("NOMBRE")));
    ui->tblWdt_ProcesoEjec->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("INFORMACION")));
    ui->tblWdt_ProcesoEjec->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.

    ui->tblWdt_Terminados->setColumnCount(4);
    ui->tblWdt_Terminados->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("OPERACION")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("RESULTADO")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("LOTE")));
    ui->tblWdt_Terminados->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.

    connect(ui->btn_Enviar, &QPushButton::clicked, this, &MainWindow::sendData);
}

MainWindow::~MainWindow()
{
    delete ui;

    for(QVector<Batch*>::iterator it = batches.begin(); it != batches.end(); ++it) {
        delete (*it);
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

        int tiempoMaximoEst = ui->spnBx_TME->value();
        process->setTiempoMaximoEst(tiempoMaximoEst);

        int id = ui->spnBx_ID->value();

        if(validID(id)) {
            process->setId(id);

            if(++processInserted > LIMITE_PROCESO) {
                ++index;
                process->setNumBatch(++batchNum);
                ui->lcd_LotesRestantes->display(batchNum);
                processInserted = 1;
            } else {
                process->setNumBatch(batchNum);
            }
            batches.at(index)->insertProcess(process);
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

int MainWindow::computebatcheses(int numProcess) {
    int batchesCount = 0;
    if(numProcess % 4 == 0) {
        batchesCount = numProcess / 4;
    } else {
        batchesCount = numProcess / 4;
        ++batchesCount;
    }

    return batchesCount;
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

void MainWindow::updateGlobalCounter(int& value)
{
//    ui->lcd_ContGlobal->display(++value);
}

void MainWindow::sendData()
{
    if(!onlyOnce) {
        onlyOnce = true;
        batchesCount = computebatcheses(ui->spnBx_CantProcesos->value());
        ui->lcd_LotesRestantes->display(batchesCount);
    }

    if(!firstTime) {
//        batches.clear(); // without deleting, you can validate any id from any batch.
        processRemaining = ui->spnBx_CantProcesos->value();
        firstTime = true;
        for(int i = 0; i < batchesCount; ++i) {
            Batch *batch = new Batch;
            batches.push_back(batch);
        }
    }

    ui->spnBx_CantProcesos->setEnabled(false);

    if(processInserted == LIMITE_PROCESO) {
        insertProcess(indexBatch);
        batches.at(indexBatch)->setSize(1); // Update the batch size.
    } else {
        insertProcess(indexBatch);
    }

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

void MainWindow::run()
{
//    qDebug() << "\n\n";
//    qDebug() << "Cantidad de lotes: " << batches.size();
//    for(const auto& batch : batches) {
//        for(const auto& process : batch->getProcesses()) {
//            int aux = 1;
//            thread->start();
////            threadGlobalCounter->start();
//            for(int i = 0; i < process->getTiempoMaximoEst(); ++i) {
//                qDebug() << aux++;
//                updateGlobalCounter(globalCounter);
//                thread->sleep(1);
//            }
//        }
    //    }
}

QVector<Batch *> MainWindow::getBatches() const
{
    return batches;
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

        threadGlobalCounter->start();

//        threadGlobalCounter->start();
//        thread->start();

        ui->ldt_NombProgr->setEnabled(true);
        ui->ldt_Operacion->setEnabled(true);
        ui->spnBx_ID->setEnabled(true);
        ui->spnBx_TME->setEnabled(true);
        ui->spnBx_CantProcesos->setEnabled(true);
        ui->btn_Enviar->setEnabled(true);
    } else {
        QMessageBox::information(this, tr("Lotes Vacios"), tr("No hay procesos, inserte procesos para empezar analizar los lotes"));
    }
}
