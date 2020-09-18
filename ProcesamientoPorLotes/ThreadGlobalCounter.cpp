#include "ThreadGlobalCounter.h"

ThreadGlobalCounter::ThreadGlobalCounter(QThread *parent) :
    QThread(parent)
  , tiempoMaxEst(0)
  , sum(0)
{
}

void ThreadGlobalCounter::run()
{
    for(const auto tiempoMaximo : tiemposEstimados) {
        for(int i = 0; i < tiempoMaximo; ++i) {
            qDebug() << "i: " << i;
            sum += i;
            emit updateCounter(sum);
            sleep(1);
        }
    }
}
