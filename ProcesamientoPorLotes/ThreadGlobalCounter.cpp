#include "ThreadGlobalCounter.h"

ThreadGlobalCounter::ThreadGlobalCounter(QThread *parent) :
    QThread(parent)
  , globalCounter(0)
{
}

void ThreadGlobalCounter::run()
{
    for(const auto& TME : tiemposEstimados) {
        for(int i = 0; i < TME; ++i) {
            emit updateCounter(++globalCounter);
            sleep(1);
        }
    }
    tiemposEstimados.clear();
}
