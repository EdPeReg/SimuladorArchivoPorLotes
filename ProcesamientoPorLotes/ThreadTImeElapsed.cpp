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
    pauseRequired = true;
}

void ThreadTImeElapsed::resume() {
    pauseRequired = false;
}

void ThreadTImeElapsed::run() {
    for(const auto& TME : tiemposEstimados) {
        int counter = 0;

        for(int i = 0; i < TME; ++i) {
            if(!pauseRequired) {
                emit updateCounter(++counter);
                sleep(1);
            } else {
                break;
            }
        }
        if(pauseRequired) break;
    }

    if(!pauseRequired) {
        tiemposEstimados.clear();
    }
}
