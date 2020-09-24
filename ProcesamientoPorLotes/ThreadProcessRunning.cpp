#include "ThreadProcessRunning.h"

ThreadProcessRunning::ThreadProcessRunning(QThread *parent) :
    QThread(parent)
{

}

void ThreadProcessRunning::run() {
    for(const auto& batch : batches) {
        updateTableCurrentBatch(batch);

        for(const auto& process : batch->getProcesses()) {
            emit updateTable(process);
            sleep(process->getTiempoMaximoEst());
            emit updateTableFinish(process);
        }
    }

//    for(const auto& process : processes) {
//        emit updateTable(process);
//        sleep(process->getTiempoMaximoEst());
//        emit updateTableFinish(process);
//    }

    batches.clear();
    emit reset();
}
