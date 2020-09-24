#include "ThreadTables.h"

ThreadProcessRunning::ThreadProcessRunning(QThread *parent) :
    QThread(parent)
{

}

void ThreadProcessRunning::run() {
    for(const auto& batch : batches) {
        updateTableCurrentBatch(batch);

        for(const auto& process : batch->getProcesses()) {
            emit updateTableProcessRunning(process);
            sleep(process->getTiempoMaximoEst());
            emit updateTableFinish(process);
        }
    }

    batches.clear();
    emit reset();
}
