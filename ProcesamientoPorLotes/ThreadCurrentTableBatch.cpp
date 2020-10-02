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
    pauseRequired = true;
}

void ThreadCurrentTableBatch::resume() {
    pauseRequired = false;
}

void ThreadCurrentTableBatch::run()
{
    for(const auto& batch : batches) {
        if(!pauseRequired) {
            emit updateTableCurrentBatch(batch);
            for(const auto& process : batch->getProcesses()) {
                sleep(process->getTiempoMaximoEst());
            }
        } else {
            break;
        }
    }

    if(!pauseRequired) {
        batches.clear();
    }
}
