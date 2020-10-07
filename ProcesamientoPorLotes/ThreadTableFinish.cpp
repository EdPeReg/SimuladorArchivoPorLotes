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
    sync.lock();
    pauseRequired = true;
    sync.unlock();
}

void ThreadTableFinish::resume() {
    sync.lock();
    pauseRequired = false;
    sync.unlock();
    pauseCond.wakeAll();
}

void ThreadTableFinish::run()
{
    for(const auto& batch : batches) {
        for(const auto& process : batch->getProcesses()) {
            sleep(process->getTiempoMaximoEst());

            sync.lock();
            if(pauseRequired)
                pauseCond.wait(&sync);
            sync.unlock();

            emit updateTableFinish(process);
        }
    }

    if(!pauseRequired) {
        emit reset();
        batches.clear();
    }
}
