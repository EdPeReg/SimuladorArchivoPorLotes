#include "DialogCounters.h"
#include "ui_dialogcounters.h"

DialogCounters::DialogCounters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCounters)
{
    ui->setupUi(this);

    threadGlobalCounter = new ThreadGlobalCounter;

    threadGlobalCounter->start();
//    connect(threadGlobalCounter, &ThreadGlobalCounter::updateGlobalCounter, this, &DialogCounters::updateGlobalCounter);
}

DialogCounters::~DialogCounters()
{
    delete ui;
}

void DialogCounters::updateGlobalCounter(int n)
{
//    ui->lcdNumber->display(n);
}
