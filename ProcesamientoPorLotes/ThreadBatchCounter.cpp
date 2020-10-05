#include "ThreadBatchCounter.h"

ThreadBatchCounter::ThreadBatchCounter(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
  , indexBatch(0)
  , indexProcess(0)
  , currentBatchCounter(0)
{

}

void ThreadBatchCounter::setBatch(Batch *batch) {
    batches.push_back(batch);
}

void ThreadBatchCounter::pause() {
    sync.lock();
    pauseRequired = true;
    sync.unlock();
}

void ThreadBatchCounter::resume()
{
    sync.lock();
    pauseRequired = false;
    sync.unlock();
    pauseCond.wakeAll();
}

void ThreadBatchCounter::run()
{
    for(const auto& batch : batches) {
        int totalTME = 0;

        sync.lock();
        if(pauseRequired)
            pauseCond.wait(&sync);
        sync.unlock();

        for(const auto& process : batch->getProcesses()) {
            totalTME += process->getTiempoMaximoEst();
        }

        emit updateBatchCounter(--currentBatchCounter);
        sleep(totalTME);
    }

    if(!pauseRequired) {
        batches.clear();
    }
}
