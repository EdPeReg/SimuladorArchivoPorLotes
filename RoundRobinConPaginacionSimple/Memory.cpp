#include "Memory.h"
#include "ui_memory.h"

Memory::Memory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Memory)
{
    ui->setupUi(this);

    // Table 1.
    ui->tbl_wdt_01->setColumnCount(2);
    ui->tbl_wdt_01->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("No. Marco")));
    ui->tbl_wdt_01->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("")));
    ui->tbl_wdt_01->horizontalHeader()->setStretchLastSection(true);

    // Insert rows.
    ui->tbl_wdt_01->setRowCount(20);
    int i = 0;
    int row = 0;
    while(i <= 38) {
        ui->tbl_wdt_01->setVerticalHeaderItem(row++, new QTableWidgetItem(QString::number(i)));
        i += 2;
    }

    // Table 2.
    ui->tbl_wdt_02->setColumnCount(2);
    ui->tbl_wdt_02->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("No. Marco")));
    ui->tbl_wdt_02->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("")));
    ui->tbl_wdt_02->horizontalHeader()->setStretchLastSection(true);

    // Insert rows.
    ui->tbl_wdt_02->setRowCount(20);
    i = 1;
    row = 0;
    while(i <= 39) {
        ui->tbl_wdt_02->setVerticalHeaderItem(row++, new QTableWidgetItem(QString::number(i)));
        i += 2;
    }
}

Memory::~Memory()
{
    delete ui;
}
