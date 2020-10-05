#include "ThreadTImeElapsed.h"

ThreadTImeElapsed::ThreadTImeElapsed(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
{

}

void ThreadTImeElapsed::setTME(int TME) {
    tiemposEstimados.push_back(TME);
}

void ThreadTImeElapsed::pause() { // new
    sync.lock();
    pauseRequired = true;
    sync.unlock();
}

void ThreadTImeElapsed::resume() {
    sync.lock();
    pauseRequired = false;
    sync.unlock();
    pauseCond.wakeAll();
}

void ThreadTImeElapsed::run() {
    for(const auto& TME : tiemposEstimados) {
        int counter = 0;

        for(int i = 0; i < TME; ++i) {
            sync.lock();
            if(pauseRequired)
                pauseCond.wait(&sync);
            sync.unlock();

            emit updateCounter(++counter);
            sleep(1);
        }
    }

    if(!pauseRequired) {
        tiemposEstimados.clear();
    }
}
