#include "ThreadBatchCounter.h"

ThreadBatchCounter::ThreadBatchCounter(QThread *parent) :
    QThread(parent)
  , stop(false)
  , currentBatchCounter(0)
{

}

void ThreadBatchCounter::run()
{
    stop = false;
    qDebug() << "current batch counter; " << currentBatchCounter;
    for(const auto& batch : batches) {
        if(!stop) {
            int totalTME = 0;
            for(const auto& process : batch->getProcesses()) {
                totalTME += process->getTiempoMaximoEst();
            }
            emit updateBatchCounter(--currentBatchCounter);
            sleep(totalTME);
        } else {
            break;
        }
    }
    batches.clear();
}
