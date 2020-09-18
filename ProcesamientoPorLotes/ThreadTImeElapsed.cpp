#include "ThreadTImeElapsed.h"

ThreadTImeElapsed::ThreadTImeElapsed(QThread *parent) :
    QThread(parent)
{

}

void ThreadTImeElapsed::run() {
    for(const auto& TME : tiemposEstimados) {
        int counter = 0;
        for(int i = 0; i < TME; ++i) {
            emit updateCounter(++counter);
            sleep(1);
        }
    }
    tiemposEstimados.clear();
}
