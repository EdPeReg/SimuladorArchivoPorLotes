#include "mainwindow.h"
#include "ui_mainwindow.h"

// TODO
// VALIDAR ID Y QUE LCA CANTIDAD DE LOTES SE ACTUALICE CORRECTAMENTE
// BUSCAR MANERA DE EVITAR MEMORY LEAKS

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , errorOperation(false)
    , errorID(false)
    , firstTime(false)
    , processInserted(0)
    , processRemaining(0)
    , batchesCount(0)
    , indexProcess(1)
{
    ui->setupUi(this);

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

void MainWindow::sendData()
{
    batchesCount = computebatcheses(ui->spnBx_CantProcesos->value());
    Process* process = new Process;

    if(!firstTime) {
        batches.clear();

        processRemaining = ui->spnBx_CantProcesos->value();
        firstTime = true;
        for(int i = 0; i < batchesCount; ++i) {
            Batch *batch = new Batch;
            batches.push_back(batch);
        }
    }

    ui->spnBx_CantProcesos->setEnabled(false);

    int index = 0;
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
        Batch* b = batches.at(index);
        QVector<Process*> p = b->getProcesses();
        for(int i = 0; i < p.size(); ++i) {
            if(p.at(i)->getId() == id) {
                QMessageBox::warning(this, tr("errorOperation, ID YA EXISTENTE"), tr("El ID ingresado ya existe, por favor ingrese otra ID distinta"));
                ui->spnBx_ID->clear();
                errorID = true;
                break;
            } else {
                errorID = false;
            }
        }

        if(!errorID) {
            process->setId(id);

            batches.at(index)->insertProcess(process);
            if(++processInserted > LIMITE_PROCESO) {
                qDebug() << "cambiando lote: " << indexProcess;
                ++index;
                process->setNumBatch(++indexProcess);
                processInserted = 1;
            } else {
                qDebug() << "mismo lote: " << indexProcess;
                process->setNumBatch(indexProcess);
            }

            ui->lcd_LotesRestantes->display(batchesCount);
            ui->lcd_ProcRestante->display(--processRemaining);
            if(ui->lcd_ProcRestante->value() == 0) {
                ui->spnBx_CantProcesos->setValue(0);
                ui->spnBx_CantProcesos->setEnabled(true);
                ui->lcd_LotesRestantes->display(0);
                firstTime = false;
            }

            ui->ldt_NombProgr->clear();
            ui->ldt_Operacion->clear();
            ui->spnBx_TME->setValue(0);
            ui->spnBx_ID->setValue(0);
        }
    }

    for(auto it = batches.begin(); it != batches.end(); ++it) {
        (*it)->showProccesses();
    }
}

