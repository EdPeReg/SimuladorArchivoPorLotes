#include "ThreadGlobalCounter.h"

ThreadGlobalCounter::ThreadGlobalCounter(QThread *parent) : QThread(parent)
{
}

void ThreadGlobalCounter::run()
{
    qDebug() << "running thread global counter;";
    emit updateGlobalCounter();
}
