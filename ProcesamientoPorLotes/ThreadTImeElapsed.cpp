#include "ThreadTImeElapsed.h"

ThreadTImeElapsed::ThreadTImeElapsed(QThread *parent) :
    QThread(parent)
  , stop(false)
{

}

void ThreadTImeElapsed::run() {
    stop = false;
    for(const auto& TME : tiemposEstimados) {
        int counter = 0;
            for(int i = 0; i < TME; ++i) {
                if(!stop) {
                    emit updateCounter(++counter);
                    sleep(1);
                } else {
                    break;
            }
        }
    }
    tiemposEstimados.clear();
}
