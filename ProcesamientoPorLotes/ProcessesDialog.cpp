#include "ProcessesDialog.h"
#include "ui_processesdialog.h"

ProcessesDialog::ProcessesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessesDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("Informacion de procesos terminados"));

    ui->tblWdt_Terminados->setColumnCount(5);
    ui->tblWdt_Terminados->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("NOMBRE")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("OPERACION")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("RESULTADO")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("TME")));
    ui->tblWdt_Terminados->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.
}

ProcessesDialog::~ProcessesDialog()
{
    delete ui;
}

void ProcessesDialog::setProcessesFinished(const std::vector<Process> &processes)
{
    int totalRows = processes.size();
    int row = 0;
    ui->tblWdt_Terminados->setRowCount(totalRows--);
    for(const auto& process : processes) {
        QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(process.getId()));
        QTableWidgetItem *itemName = new QTableWidgetItem(process.getProgrammerName());
        QTableWidgetItem *itemOperation = new QTableWidgetItem(process.getOperation());
        QTableWidgetItem *itemResult = new QTableWidgetItem(process.getResult());
        QTableWidgetItem *itemTME = new QTableWidgetItem(QString::number(process.getTiempoMaximoEst()));
        ui->tblWdt_Terminados->setItem(row, ID_PI, itemID);
        ui->tblWdt_Terminados->setItem(row, NAME_PI, itemName);
        ui->tblWdt_Terminados->setItem(row, OPERATION_PI, itemOperation);
        ui->tblWdt_Terminados->setItem(row, RESULT_PI, itemResult);
        ui->tblWdt_Terminados->setItem(row++, RESULT_PI, itemTME);
    }
}
