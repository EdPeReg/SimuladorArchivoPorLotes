#include "ThreadBatchCounter.h"

ThreadBatchCounter::ThreadBatchCounter(QThread *parent) :
    QThread(parent)
  , currentBatchCounter(0)
  , finish(false)
{

}

void ThreadBatchCounter::run()
{
//    if(!finish) {
        for(const auto& batch : batches) {
            int totalTME = 0;
            for(const auto& process : batch->getProcesses()) {
                totalTME += process->getTiempoMaximoEst();
            }
            emit updateBatchCounter(--currentBatchCounter);
            sleep(totalTME);
        }
//    }

//    clean();
}
