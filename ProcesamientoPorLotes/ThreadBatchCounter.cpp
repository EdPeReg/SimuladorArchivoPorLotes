#include "ThreadBatchCounter.h"

ThreadBatchCounter::ThreadBatchCounter(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
  , currentBatchCounter(0)
{

}

void ThreadBatchCounter::setBatch(Batch *batch) {
    batches.push_back(batch);
}

void ThreadBatchCounter::pause() {
    pauseRequired = true;
}

void ThreadBatchCounter::resume()
{
    pauseRequired = false;
}

void ThreadBatchCounter::run()
{
    for(const auto& batch : batches) {
        int totalTME = 0;
        if(!pauseRequired) {
            for(const auto& process : batch->getProcesses()) {
                totalTME += process->getTiempoMaximoEst();
            }

            emit updateBatchCounter(--currentBatchCounter);
            sleep(totalTME);
        } else {
            break;
        }
    }

    if(!pauseRequired) {
        batches.clear();
    }
}
