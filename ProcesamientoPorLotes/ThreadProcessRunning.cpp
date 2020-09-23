#include "ThreadProcessRunning.h"

ThreadProcessRunning::ThreadProcessRunning(QThread *parent) :
    QThread(parent)
{

}

void ThreadProcessRunning::run() {
    for(const auto& process : processes) {
        qDebug() << process;
        emit updateTable(process);
        sleep(process->getTiempoMaximoEst());
        emit updateTableFinish(process);
    }

    processes.clear();
    emit reset();
}
