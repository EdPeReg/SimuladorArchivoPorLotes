#include "Memory.h"
#include "ui_memory.h"

#include <QDebug>

Memory::Memory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Memory),
    processesLeft(0),
    procLeftCopy(0),
    row(2)
{
    ui->setupUi(this);

    ui->tbl_wdt_01->setIconSize(QSize(200,200));
    setTable();

    // First two processes are for OS.
    setColor(0, 1);
    setColor(1, 1);
}

Memory::~Memory()
{
    delete ui;
}

void Memory::insertTable(std::deque<Process> processes)
{
    // Sort by id to mantain the order. This is because at this point, using our deque
    // will change the order.
    std::sort(processes.begin(), processes.end());
    for(size_t i = 0; i < processes.size(); ++i) {
        if(processes.at(i).getEstado() == "EJECUCION") {
            insertRunningProcess(processes.at(i));
        } else if(processes.at(i).getEstado() == "LISTOS") {
            insertListos(processes.at(i));
        }
    }
}

void Memory::insertRunningProcess(const Process &process)
{
    short processSize = process.getTamano();
    int numPages = getPagesNum(processSize);

    qDebug() << "";
    qDebug() << "processes left: " << processesLeft;
    qDebug() << "Dentro de insert running";
    qDebug() << "process id: " << process.getId();
    qDebug() << "process estado: " << process.getEstado();
    qDebug() << "process size: " << process.getTamano();
    qDebug() << "num pages: " << numPages;
    qDebug() << "remainder: " << processSize;

    while(numPages--) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setIcon(QIcon("images/rojo/rojo_lleno.png"));
        ui->tbl_wdt_01->setItem(row++, 1, item);
    }

    switch(processSize) {
        case 1: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/rojo/rojo_01.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
        break;
        }

        case 2: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/rojo/rojo_02.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
        break;
        }

        case 3: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/rojo/rojo_03.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
        break;
        }

        case 4: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/rojo/rojo_04.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
        break;
        }
    }

    // Start again from the beggining in our table.
    if(--processesLeft == 0) {
        row = 2;
        processesLeft = procLeftCopy;
    }
}

void Memory::insertListos(const Process& process)
{
//    listos.pop_front();
//    for(size_t i = 0; i < listos.size(); ++i) {
        short processSize = process.getTamano();
        int numPages = getPagesNum(processSize);

        qDebug() << "";
        qDebug() << "dentro de insert listos";
        qDebug() << "process id: " << process.getId();
        qDebug() << "process estado: " << process.getEstado();
        qDebug() << "process size: " << process.getTamano();
        qDebug() << "num pages: " << numPages;
        qDebug() << "remainder: " << processSize;

        while(numPages--) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/azul/azul_lleno.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
        }

        switch(processSize) {
            case 1: {
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setIcon(QIcon("images/azul/azul_01.png"));
                ui->tbl_wdt_01->setItem(row++, 1, item);
            break;
            }

            case 2: {
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setIcon(QIcon("images/azul/azul_02.png"));
                ui->tbl_wdt_01->setItem(row++, 1, item);
            break;
            }

            case 3: {
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setIcon(QIcon("images/azul/azul_03.png"));
                ui->tbl_wdt_01->setItem(row++, 1, item);
            break;
            }

            case 4: {
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setIcon(QIcon("images/azul/azul_04.png"));
                ui->tbl_wdt_01->setItem(row++, 1, item);
            break;
            }
        }
//    }

    // Start again from the beggining in our table.
    if(--processesLeft == 0) {
        row = 2;
        processesLeft = procLeftCopy;
    }
}

void Memory::setTable() {
    ui->tbl_wdt_01->setColumnCount(2);
    ui->tbl_wdt_01->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("No. Marco")));
    ui->tbl_wdt_01->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("")));
    ui->tbl_wdt_01->horizontalHeader()->setStretchLastSection(true);

    // Insert rows.
    ui->tbl_wdt_01->setRowCount(40);
    int i = 0;
    int row = 0;
    QFont font;
    font.setBold(true);

    while(i <= 40) {
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(i++));
        item->setFont(font);

        // To delete the default number that appear when a row is created.
        ui->tbl_wdt_01->setVerticalHeaderItem(row, new QTableWidgetItem());
        ui->tbl_wdt_01->setItem(row++, 0, item);
    }
}

void Memory::setColor(int row, int col) {
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setBackgroundColor(Qt::black);
    ui->tbl_wdt_01->setItem(row, col, item);
}

void Memory::setProcessesLeft(int processesLeft)
{
    this->processesLeft = processesLeft;
}

void Memory::setProcLeftCopy(int procLeftCopy)
{
    this->procLeftCopy = procLeftCopy;
}

void Memory::setRow(int row)
{
    this->row = row;
}

void Memory::clearRow()
{
    ui->tbl_wdt_01->setRowCount(0);
    setTable();

    // First two processes are for OS.
    setColor(0, 1);
    setColor(1, 1);
}

short Memory::getPagesNum(short& size)
{
    short numPages = size / TAMANO_PAGINA;
    size %= 5; // Get how many pages are left.
    return numPages;
}
