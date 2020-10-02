#include "ThreadGlobalCounter.h"

#include <QDebug>


ThreadGlobalCounter::ThreadGlobalCounter(QThread *parent) :
    QThread(parent)
  , stop(false)
  , globalCounter(0)
  , currentIndex(0)
{
}

void ThreadGlobalCounter::run()
{
    stop = false;
    qDebug() << "thread global counter";
    for(int i = currentIndex; i < tiemposEstimados.size(); ++i) {
        qDebug() << "i: " << i;
        for(int j = 0; j < tiemposEstimados.at(i); ++j) {
            if(!stop) {
                emit updateCounter(++globalCounter);
                sleep(1);
            } else {
                currentIndex = i;
                tiemposEstimados[i] = tiemposEstimados.at(i) - j; // Update TME
                qDebug() << "j: " << j;
                break;
            }
        }
        if(stop) break;
    }

    if(!stop) {
        qDebug() << "cleaning";
        tiemposEstimados.clear();
    } else {
        qDebug() << "indice de parada: " << currentIndex;
        qDebug() << tiemposEstimados;
    }
}
