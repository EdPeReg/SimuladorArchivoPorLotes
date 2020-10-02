#include "ThreadTableFinish.h"

ThreadTableFinish::ThreadTableFinish(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
{

}

void ThreadTableFinish::setBatch(Batch *b) {
    batches.push_back(b);
}

void ThreadTableFinish::pause() {
    pauseRequired = true;
}

void ThreadTableFinish::resume() {
    pauseRequired = false;
}

void ThreadTableFinish::run()
{
    for(const auto& batch : batches) {
        for(const auto& process : batch->getProcesses()) {
            sleep(process->getTiempoMaximoEst());

            if(!pauseRequired) {
                emit updateTableFinish(process);
            } else {
                break;
            }
        }
        if(pauseRequired) break;
    }

    if(!pauseRequired) {
        emit reset();
        batches.clear();
    }
}
