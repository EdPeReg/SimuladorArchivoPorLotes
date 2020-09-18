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

//    emit updateTableRunningProcess();
    // GLOBAL COUNTER.
//    for(const auto tiempoMaximo : tiemposEstimados) {
//        for(int i = 0; i < tiempoMaximo; ++i) {
//            qDebug() << "i: " << i;
//            sum += i;
//            emit updateCounter(sum);
//            sleep(1);
//        }
//    }
}
