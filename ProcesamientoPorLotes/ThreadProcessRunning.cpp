#include "ThreadProcessRunning.h"

ThreadProcessRunning::ThreadProcessRunning(QThread *parent) :
    QThread(parent)
  , finish(false)
{

}

void ThreadProcessRunning::run() {
    if(!finish) {
        for(const auto& process : processes) {
            emit updateTable(process);
            sleep(process->getTiempoMaximoEst());
            emit updateTableFinish(process);
        }
    }

    emit reset();
    processes.clear();
}
