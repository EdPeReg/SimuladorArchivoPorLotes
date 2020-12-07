#include "Memory.h"
#include "ui_memory.h"

#include <QDebug>

Memory::Memory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Memory),
    processesLeft(0),
    procLeftCopy(0),
    row(2),
    repeated(false)
{
    ui->setupUi(this);

    ui->tbl_wdt_01->setIconSize(QSize(200,200));
    setTable();

    // First two listos are for OS.
    setColor(0, 1);
    setColor(1, 1);
}

Memory::~Memory()
{
    delete ui;
}

void Memory::insertTable(std::deque<Process> listos, std::deque<Process> bloqueados)
{
    // Sort by id to mantain the order. This is because at this point, using our deque
    // will change the order.
    std::sort(listos.begin(), listos.end());
    std::sort(bloqueados.begin(), bloqueados.end());

    processesLeft = listos.size();
    for(auto process : listos) {
         if(process.getEstado() == "EJECUCION") {
            insertRunningProcess(process, listos.size());
        } else if(process.getEstado() == "LISTOS") {
            insertListos(process, listos.size());
        } //else if(process.getEstado() == "BLOQUEADO") {
          //  insertBloqueado(process);
        //}
    }

    for(auto process : bloqueados) {
        insertBloqueado(process);
    }
}

void Memory::insertRunningProcess(const Process &process, int size)
{
    Utility utility;
    short processSize = process.getTamano();
    int numPages = getPagesNum(processSize);
    bool repeated = false;

    qDebug() << "";
    qDebug() << "listos left: " << processesLeft;
    qDebug() << "Dentro de insert running";
    qDebug() << "process id: " << process.getId();
    qDebug() << "process estado: " << process.getEstado();
    qDebug() << "process size: " << process.getTamano();
    qDebug() << "num pages: " << numPages;
    qDebug() << "remainder: " << processSize;
    qDebug() << "row: " << row;

    // Set the flag to not repeat elements.
    for(const auto& e : utilidades) {
        if(e.processID == process.getId()) {
            repeated = true;
            break;
        }
    }

    // Save our start position from this process.
    if(!repeated) {
        utility.processID = process.getId();
        utility.start = row;
    }

    // Get the start proper position of our process.
    for(const auto& e : utilidades) {
        if(e.processID == process.getId()) {
            row = e.start;
            break;
        }
    }

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
            if(!repeated) {
                utility.end = row;
            }
        break;
        }

        case 2: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/rojo/rojo_02.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
            if(!repeated) {
                utility.end = row;
            }
        break;
        }

        case 3: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/rojo/rojo_03.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
            if(!repeated) {
                utility.end = row;
            }
        break;
        }

        case 4: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/rojo/rojo_04.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
            if(!repeated) {
                utility.end = row;
            }
        break;
        }
    }

    // If pages are completed, start in the next position.
    if(processSize == 0) {
        utility.end = row + 1;;
    }

    // Save our start position from this process.
    if(!repeated) {
        utilidades.push_back(utility);
    }

    // Start again from the beggining in our table.
    if(--processesLeft == 0) {
        row = 2;
    }

    for(const auto& e : utilidades) {
        qDebug() << e.processID;
        qDebug() << e.start;
        qDebug() << e.end;
    }
}

void Memory::insertListos(const Process& process, int size)
{
    Utility utility;
    short processSize = process.getTamano();
    int numPages = getPagesNum(processSize);
    bool repeated = false;

    qDebug() << "";
    qDebug() << "dentro de insert listos";
    qDebug() << "process id: " << process.getId();
    qDebug() << "listos left: " << processesLeft;
    qDebug() << "process estado: " << process.getEstado();
    qDebug() << "process size: " << process.getTamano();
    qDebug() << "num pages: " << numPages;
    qDebug() << "remainder: " << processSize;
    qDebug() << "row: " << row;

    // Set the flag to not repeat elements.
    for(const auto& e : utilidades) {
        if(e.processID == process.getId()) {
            repeated = true;
            break;
        }
    }

    if(!repeated) {
        utility.processID = process.getId();
        utility.start = row;
    }

    // Get the start proper position of our process.
    for(const auto& e : utilidades) {
        if(e.processID == process.getId()) {
            row = e.start;
            break;
        }
    }

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
            if(!repeated) {
                utility.end = row;
            }
        break;
        }

        case 2: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/azul/azul_02.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
            if(!repeated) {
                utility.end = row;
            }
        break;
        }

        case 3: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/azul/azul_03.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
            if(!repeated) {
                utility.end = row;
            }
        break;
        }

        case 4: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/azul/azul_04.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
            if(!repeated) {
                utility.end = row;
            }
        break;
        }
    }

    // If pages are completed, start in the next position.
    if(processSize == 0) {
        utility.end = row + 1;;
    }

    if(!repeated) {
        utilidades.push_back(utility);
    }

    // Start again from the beggining in our table.
    if(--processesLeft == 0) {
        row = 2;
    }

    for(const auto& e : utilidades) {
        qDebug() << e.processID;
        qDebug() << e.start;
        qDebug() << e.end;
    }
}

void Memory::insertBloqueado(const Process &process)
{
    short processSize = process.getTamano();
    int numPages = getPagesNum(processSize);
    int rowStart, rowEnd;

    for(const auto& e : utilidades) {
        if(e.processID == process.getId()) {
            row = e.start;
            rowEnd = e.end;
            break;
        }
    }

    qDebug() << "";
    qDebug() << "listos left: " << processesLeft;
    qDebug() << "Dentro de insert running";
    qDebug() << "process id: " << process.getId();
    qDebug() << "process estado: " << process.getEstado();
    qDebug() << "process size: " << process.getTamano();
    qDebug() << "num pages: " << numPages;
    qDebug() << "remainder: " << processSize;

    while(numPages--) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setIcon(QIcon("images/verde/verde_lleno.png"));
        ui->tbl_wdt_01->setItem(row++, 1, item);
    }

    switch(processSize) {
        case 1: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/verde/verde_01.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
        break;
        }

        case 2: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/verde/verde_02.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
        break;
        }

        case 3: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/verde/verde_03.png"));
            ui->tbl_wdt_01->setItem(row++, 1, item);
        break;
        }

        case 4: {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setIcon(QIcon("images/verde/verde_04.png"));
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
