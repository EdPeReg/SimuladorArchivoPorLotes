#include "ProcessesDialog.h"
#include "ui_processesdialog.h"

#include <QDebug>

ProcessesDialog::ProcessesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessesDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("Informacion de procesos terminados"));

    ui->tblWdt_Terminados->setColumnCount(11);
    ui->tblWdt_Terminados->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("NOMBRE")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("OPERACION")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("RESULTADO")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("TME")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Tiempo de Llegada")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(6, new QTableWidgetItem(tr("Tiempo de finalizacion")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(7, new QTableWidgetItem(tr("Tiempo de retorno")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(8, new QTableWidgetItem(tr("Tiempo de respuesta")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(9, new QTableWidgetItem(tr("Tiempo de espera")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(10, new QTableWidgetItem(tr("Tiempo de servicio")));
    ui->tblWdt_Terminados->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.
    ui->tblWdt_Terminados->resizeColumnsToContents(); // All text will fit in the column width.

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
        qDebug() << "process with ID: " << process.getId() << " global counter: " << process.getGlobalCounter();

        // It can also be tiempo de espera - tiempo de servicio.
        int _tiempoRetorno = process.getTiempoFinalizacion() - process.getTiempoLlegada();
        int _tiempoEspera = _tiempoRetorno - process.getTiempoServicio();
        QTableWidgetItem *ID = new QTableWidgetItem(QString::number(process.getId()));
        QTableWidgetItem *Name = new QTableWidgetItem(process.getProgrammerName());
        QTableWidgetItem *Operation = new QTableWidgetItem(process.getOperation());
        QTableWidgetItem *Result = new QTableWidgetItem(process.getResult());
        QTableWidgetItem *TME = new QTableWidgetItem(QString::number(process.getTiempoMaximoEst()));
        QTableWidgetItem *tiempoLlegada = new QTableWidgetItem(QString::number(process.getTiempoLlegada()));
        QTableWidgetItem *tiempoFinalizacion = new QTableWidgetItem(QString::number(process.getTiempoFinalizacion()));
        QTableWidgetItem *tiempoDeRetorno = new QTableWidgetItem(QString::number(_tiempoRetorno));
        QTableWidgetItem *tiempoDeRespuesta = new QTableWidgetItem(QString::number(process.getTiempoDeRespuesta()));
        QTableWidgetItem *tiempoEspera = new QTableWidgetItem(QString::number(_tiempoEspera));
        QTableWidgetItem *tiempoServicio = new QTableWidgetItem(QString::number(process.getTiempoServicio()));
        ui->tblWdt_Terminados->setItem(row, ID_PI, ID);
        ui->tblWdt_Terminados->setItem(row, NAME_PI, Name);
        ui->tblWdt_Terminados->setItem(row, OPERATION_PI, Operation);
        ui->tblWdt_Terminados->setItem(row, RESULT_PI, Result);
        ui->tblWdt_Terminados->setItem(row, TME_PI, TME);
        ui->tblWdt_Terminados->setItem(row, TL_PI, tiempoLlegada);
        ui->tblWdt_Terminados->setItem(row, TF_PI, tiempoFinalizacion);
        ui->tblWdt_Terminados->setItem(row, TR_PI, tiempoDeRetorno);
        ui->tblWdt_Terminados->setItem(row, TRE_PI, tiempoDeRespuesta);
        ui->tblWdt_Terminados->setItem(row, TE_PI, tiempoEspera);
        ui->tblWdt_Terminados->setItem(row++, TS_PI, tiempoServicio);
    }
}
