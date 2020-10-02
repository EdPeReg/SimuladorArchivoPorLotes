#include "ThreadCurrentTableBatch.h"

ThreadCurrentTableBatch::ThreadCurrentTableBatch(QThread *parent) : QThread(parent)
{

}

void ThreadCurrentTableBatch::run()
{
    for(const auto& batch : batches) {
        emit updateTableCurrentBatch(batch);
        for(const auto& process : batch->getProcesses()) {
            sleep(process->getTiempoMaximoEst());
        }
    }
    batches.clear();
}
