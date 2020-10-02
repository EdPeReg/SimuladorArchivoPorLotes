#include "ThreadTableRunning.h"

ThreadTableRunning::ThreadTableRunning(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
{

}

void ThreadTableRunning::setBatch(Batch *batch) {
    batches.push_back(batch);
}

void ThreadTableRunning::pause() {
    pauseRequired = true;
}

void ThreadTableRunning::resume() {
    pauseRequired = false;;
}

void ThreadTableRunning::run()
{
    for(const auto& batch : batches) {
        for(const auto& process : batch->getProcesses()) {
            if(!pauseRequired) {
                emit updateTableProcessRunning(process);
                sleep(process->getTiempoMaximoEst());
            } else {
                break;
            }
        }
        if(pauseRequired) break;
    }

    if(!pauseRequired) {
        batches.clear();
    }
}
