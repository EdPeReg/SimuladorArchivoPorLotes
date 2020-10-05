#include "ThreadCurrentTableBatch.h"

ThreadCurrentTableBatch::ThreadCurrentTableBatch(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
{

}

void ThreadCurrentTableBatch::setBatch(Batch *batch) {
    batches.push_back(batch);
}

void ThreadCurrentTableBatch::pause() {
    sync.lock();
    pauseRequired = true;
    sync.unlock();
}

void ThreadCurrentTableBatch::resume() {
    sync.lock();
    pauseRequired = false;
    sync.unlock();
    pauseCond.wakeAll();
}

void ThreadCurrentTableBatch::run()
{
    for(const auto& batch : batches) {
        if(pauseRequired)
            pauseCond.wait(&sync);

        emit updateTableCurrentBatch(batch);
        for(const auto& process : batch->getProcesses()) {
            sleep(process->getTiempoMaximoEst());
        }
    }

    if(!pauseRequired) {
        batches.clear();
    }
}
