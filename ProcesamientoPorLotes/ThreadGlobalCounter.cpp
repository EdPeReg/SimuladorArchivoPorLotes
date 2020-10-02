#include "ThreadGlobalCounter.h"

#include <QDebug>


ThreadGlobalCounter::ThreadGlobalCounter(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
  , globalCounter(0)
  , currentIndex(0)
{
}

void ThreadGlobalCounter::pause() { // new
    pauseRequired = true;
}

void ThreadGlobalCounter::resume()
{
    pauseRequired = false;
}

void ThreadGlobalCounter::setTiemposEstimados(int tiempoEstimado) {
    tiemposEstimados.push_back(tiempoEstimado);
}

void ThreadGlobalCounter::run()
{
    qDebug() << "thread global counter";

        for(int i = currentIndex; i < tiemposEstimados.size(); ++i) {
            for(int j = 0; j < tiemposEstimados.at(i); ++j) {
                if(!pauseRequired) {
                    emit updateCounter(++globalCounter);
                    sleep(1);
                } else {
                    break;
                }



    //            if(!pauseRequired) {
//                    emit updateCounter(++globalCounter);
//                    sleep(1);
    //            } //else {
    //                currentIndex = i;
    //                tiemposEstimados[i] = tiemposEstimados.at(i) - j; // Update TME
    //                qDebug() << "j: " << j;
    //                break;
    //            }
            }
            if(pauseRequired) break;

    //    if(!pauseRequired) {
    //    } else {
    //        qDebug() << "indice de parada: " << currentIndex;
    //        qDebug() << tiemposEstimados;
    //    }
    }

    if(!pauseRequired) {
        qDebug() << "cleaning";
        tiemposEstimados.clear();
    }
}
