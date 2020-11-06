#include "ProcessesDialog.h"
#include "ui_processesdialog.h"

ProcessesDialog::ProcessesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessesDialog)
{
    ui->setupUi(this);
    this->setFocusPolicy(Qt::StrongFocus);
    ui->tblWdt_Terminados->setFocusPolicy(Qt::NoFocus);

    setWindowTitle(tr("Informacion de procesos"));

    ui->tblWdt_Terminados->setColumnCount(13);
    ui->tblWdt_Terminados->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("NOMBRE")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("OPERACION")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("RESULTADO")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("ESTADO")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("TME")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(6, new QTableWidgetItem(tr("Tiempo de Llegada")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(7, new QTableWidgetItem(tr("Tiempo de finalizacion")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(8, new QTableWidgetItem(tr("Tiempo de retorno")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(9, new QTableWidgetItem(tr("Tiempo de respuesta")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(10, new QTableWidgetItem(tr("Tiempo de espera")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(11, new QTableWidgetItem(tr("Tiempo de servicio")));
    ui->tblWdt_Terminados->setHorizontalHeaderItem(12, new QTableWidgetItem(tr("Tiempo Restante en CPU")));
    ui->tblWdt_Terminados->horizontalHeader()->setStretchLastSection(true); // Stretches the last column to fit the remaining space.
    ui->tblWdt_Terminados->resizeColumnsToContents(); // All text will fit in the column width.
}

ProcessesDialog::~ProcessesDialog()
{
    delete ui;
}

void ProcessesDialog::setProcesses(const std::vector<Process> &processes)
{
    int totalRows = processes.size();
    int row = 0;
    ui->tblWdt_Terminados->setRowCount(totalRows--);
    for(auto& process : processes) {
        // It can also be tiempo de espera - tiempo de servicio.
        int _tiempoRetorno = process.getTiempoFinalizacion() - process.getTiempoLlegada();
        int _tiempoEspera = _tiempoRetorno - process.getTiempoServicio();

        QTableWidgetItem *ID = new QTableWidgetItem;
        QTableWidgetItem *Name = new QTableWidgetItem;
        QTableWidgetItem *Operation = new QTableWidgetItem;
        QTableWidgetItem *Result = new QTableWidgetItem;
        QTableWidgetItem *Estado = new QTableWidgetItem;
        QTableWidgetItem *TME = new QTableWidgetItem;
        QTableWidgetItem *tiempoLlegada = new QTableWidgetItem;
        QTableWidgetItem *tiempoFinalizacion = new QTableWidgetItem;
        QTableWidgetItem *tiempoDeRetorno = new QTableWidgetItem;
        QTableWidgetItem *tiempoDeRespuesta = new QTableWidgetItem;
        QTableWidgetItem *tiempoEspera = new QTableWidgetItem;
        QTableWidgetItem *tiempoServicio = new QTableWidgetItem;
        QTableWidgetItem *tiempoRestanteCPU = new QTableWidgetItem;

        if(process.getEstado() == "LISTOS" or process.getEstado() == "EJECUCION" or
           process.getEstado() == "BLOQUEADO" or process.getEstado() == "NUEVO")
        {
            ID->setText(QString::number(process.getId()));
            Name->setText(process.getProgrammerName());
            Operation->setText(process.getOperation());
            Result->setText("-");
            Estado->setText(process.getEstado());
            TME->setText(QString::number(process.getTiempoMaximoEst()));
            tiempoLlegada->setText(QString::number(process.getTiempoLlegada()));
            tiempoFinalizacion->setText("-");
            tiempoDeRetorno->setText("-");
            tiempoDeRespuesta->setText(QString::number(process.getTiempoDeRespuesta()));
            tiempoEspera->setText(QString::number(_tiempoEspera));
            tiempoServicio->setText(QString::number(process.getTiempoServicio()));
            tiempoRestanteCPU->setText(QString::number(process.getTiempoRestanteCPU()));

            ui->tblWdt_Terminados->setItem(row, ID_PI, ID);
            ui->tblWdt_Terminados->setItem(row, NAME_PI, Name);
            ui->tblWdt_Terminados->setItem(row, OPERATION_PI, Operation);
            ui->tblWdt_Terminados->setItem(row, RESULT_PI, Result);
            ui->tblWdt_Terminados->setItem(row, ESTADO_PI, Estado);
            ui->tblWdt_Terminados->setItem(row, TME_PI, TME);
            ui->tblWdt_Terminados->setItem(row, TL_PI, tiempoLlegada);
            ui->tblWdt_Terminados->setItem(row, TF_PI, tiempoFinalizacion);
            ui->tblWdt_Terminados->setItem(row, TR_PI, tiempoDeRetorno);
            ui->tblWdt_Terminados->setItem(row, TRE_PI, tiempoDeRespuesta);
            ui->tblWdt_Terminados->setItem(row, TE_PI, tiempoEspera);
            ui->tblWdt_Terminados->setItem(row, TS_PI, tiempoServicio);
            ui->tblWdt_Terminados->setItem(row++, TRCPU_PI, tiempoRestanteCPU);
        } else { // The process is not in memory anymore.
            qDebug() << "process with ID: " << process.getId() << " global counter: " << process.getGlobalCounter();

            // It can also be tiempo de espera - tiempo de servicio.
            _tiempoRetorno = process.getTiempoFinalizacion() - process.getTiempoLlegada();
            _tiempoEspera = _tiempoRetorno - process.getTiempoServicio();

            ID->setText(QString::number(process.getId()));
            Name->setText(process.getProgrammerName());
            Operation->setText(process.getOperation());
            Result->setText(process.getResult());
            Estado->setText(process.getEstado());
            TME->setText(QString::number(process.getTiempoMaximoEst()));
            tiempoLlegada->setText(QString::number(process.getTiempoLlegada()));
            tiempoFinalizacion->setText(QString::number(process.getTiempoFinalizacion()));
            tiempoDeRetorno->setText(QString::number(_tiempoRetorno));
            tiempoDeRespuesta->setText(QString::number(process.getTiempoDeRespuesta()));
            tiempoEspera->setText(QString::number(_tiempoEspera));
            tiempoServicio->setText(QString::number(process.getTiempoServicio()));
            tiempoRestanteCPU->setText("-");

            ui->tblWdt_Terminados->setItem(row, ID_PI, ID);
            ui->tblWdt_Terminados->setItem(row, NAME_PI, Name);
            ui->tblWdt_Terminados->setItem(row, OPERATION_PI, Operation);
            ui->tblWdt_Terminados->setItem(row, RESULT_PI, Result);
            ui->tblWdt_Terminados->setItem(row, ESTADO_PI, Estado);
            ui->tblWdt_Terminados->setItem(row, TME_PI, TME);
            ui->tblWdt_Terminados->setItem(row, TL_PI, tiempoLlegada);
            ui->tblWdt_Terminados->setItem(row, TF_PI, tiempoFinalizacion);
            ui->tblWdt_Terminados->setItem(row, TR_PI, tiempoDeRetorno);
            ui->tblWdt_Terminados->setItem(row, TRE_PI, tiempoDeRespuesta);
            ui->tblWdt_Terminados->setItem(row, TE_PI, tiempoEspera);
            ui->tblWdt_Terminados->setItem(row, TS_PI, tiempoServicio);
            ui->tblWdt_Terminados->setItem(row++, TRCPU_PI, tiempoRestanteCPU);
        }
    }
}

void ProcessesDialog::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
        // Continue.
        case Qt::Key_C:
            qDebug() << "CONTINUE";
            this->close(); // Close this dialog.
        break;
    }
}
