#include "ThreadTableRunning.h"

ThreadTableRunning::ThreadTableRunning(QThread *parent) : QThread(parent)
{

}

void ThreadTableRunning::run()
{
    for(const auto& batch : batches) {
        for(const auto& process : batch->getProcesses()) {
            emit updateTableProcessRunning(process);
            sleep(process->getTiempoMaximoEst());
        }
    }
    batches.clear();
}
