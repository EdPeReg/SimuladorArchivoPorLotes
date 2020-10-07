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
    sync.lock();
    pauseRequired = true;
    sync.unlock();
}

void ThreadTableRunning::resume() {
    sync.lock();
    pauseRequired = false;;
    sync.unlock();
    pauseCond.wakeAll();
}

void ThreadTableRunning::run()
{
    for(const auto& batch : batches) {
        for(const auto& process : batch->getProcesses()) {
            sync.lock();
            if(pauseRequired)
                pauseCond.wait(&sync);
            sync.unlock();

            emit updateTableProcessRunning(process);
            sleep(process->getTiempoMaximoEst());
        }
    }

    if(!pauseRequired) {
        batches.clear();
    }
}
