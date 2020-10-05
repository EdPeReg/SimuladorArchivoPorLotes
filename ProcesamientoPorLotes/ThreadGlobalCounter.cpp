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
    qDebug() << "dentroo";
    sync.lock();
    pauseRequired = true;
    sync.unlock();
}

void ThreadGlobalCounter::resume()
{
    sync.lock();
    pauseRequired = false;
    sync.unlock();
    pauseCond.wakeAll(); // Wake all threads.
}

void ThreadGlobalCounter::setTiemposEstimados(int tiempoEstimado) {
    tiemposEstimados.push_back(tiempoEstimado);
}

void ThreadGlobalCounter::run()
{
    for(int i = currentIndex; i < tiemposEstimados.size(); ++i) {
        for(int j = 0; j < tiemposEstimados.at(i); ++j) {
            sync.lock();
            if(pauseRequired) {
                qDebug() << "A pausarse se ha dichooo";
                pauseCond.wait(&sync); // Will stop to execute until shome calls resume.
            }
            sync.unlock();

            emit updateCounter(++globalCounter);
            sleep(1);
//            if(!pauseRequired) {
//                emit updateCounter(++globalCounter);
//                sleep(1);
//            } else {
//                // Update at what position will start after pausing.
//                currentIndex = i;
//                tiemposEstimados[i] = tiemposEstimados.at(i) - j; // Update TME
//                break;
//            }
        }
//        if(pauseRequired) break;
    }


    if(!pauseRequired) {
        qDebug() << "cleaning";
        tiemposEstimados.clear();
    } else {
        qDebug() << "holaaaaaaaaaaaaaaaaaaaaaa";
    }
}
