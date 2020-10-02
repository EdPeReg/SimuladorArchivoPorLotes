#include "ThreadTableFinish.h"

ThreadTableFinish::ThreadTableFinish(QThread *parent) : QThread(parent)
{

}

void ThreadTableFinish::run()
{
    for(const auto& batch : batches) {
        for(const auto& process : batch->getProcesses()) {
            sleep(process->getTiempoMaximoEst());
            emit updateTableFinish(process);
        }
    }
    emit reset();
    batches.clear();
}
