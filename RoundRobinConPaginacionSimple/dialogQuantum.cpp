#include "dialogQuantum.h"
#include "ui_dialogquantum.h"

dialogQuantum::dialogQuantum(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogQuantum)
{
    ui->setupUi(this);
}

dialogQuantum::~dialogQuantum()
{
    delete ui;
}

int dialogQuantum::getQuantum() const {
    return ui->spnBx_QuantumValue->value();
}
