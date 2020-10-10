#include "ThreadGlobalCounter.h"

#include <QDebug>

ThreadGlobalCounter::ThreadGlobalCounter(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
  , keyErrorPressed(false)
  , globalCounter(0)
  , TT(0)
  , indexTiempo(0)
{
}

void ThreadGlobalCounter::pause() { // new
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

void ThreadGlobalCounter::setTT(int TT) {
    this->TT = TT;
}

void ThreadGlobalCounter::setKeyError(bool keyErrorPressed)
{
    this->keyErrorPressed = keyErrorPressed;
}

int ThreadGlobalCounter::getTT() {
    return TT;
}

void ThreadGlobalCounter::run()
{
    int indexTime = 0;
    // Iterate un our list.
    while(indexTime < tiemposEstimados.size()) {
        qDebug() << "index time: " << indexTime;
        int j = 0;
        while(j < tiemposEstimados.at(indexTime)) {
            if(!keyErrorPressed) {
                emit updateCounter(++globalCounter);
            }
            qDebug() << "global counter: " << globalCounter;
            sleep(1);

            sync.lock();
            if(pauseRequired) {
                pauseCond.wait(&sync); // Will stop to execute until shome calls resume.

            }
            sync.unlock();

            if(keyErrorPressed) {
                tiemposEstimados[indexTime] = TT;
//                --globalCounter;
                break;
                //int newTME = tiemposEstimados.at(indexTime) - TT;
            }
            ++j;
        }
        ++indexTime;
        keyErrorPressed = false;
    }

    for(const auto& time : tiemposEstimados) {
        qDebug() << "time: " << time;
    }

    // Iterate in our list.
//    for(int i = 0; i < tiemposEstimados.size(); ++i) {
//        // Iterate in our TME.
//        int j = 0;
//        while(j < tiemposEstimados.at(i)) {
//            emit updateCounter(++globalCounter);
//            sleep(1);

//            sync.lock();
//            if(pauseRequired) {
//                pauseCond.wait(&sync); // Will stop to execute until shome calls resume.

//            }
//            sync.unlock();

//            // If there is an error in the process, let's start from where it was paused.
//            if(keyErrorPressed) {
//                qDebug() << "paused with a TME: " << tiemposEstimados.at(i);
//                int newTME = tiemposEstimados.at(i) - TT;
////                tiemposEstimados[i] = newTME;
////                qDebug() << "it should start from: " << tiemposEstimados[i];
//                keyErrorPressed = false;

////                for(const auto tiempo : tiemposEstimados) {
////                    qDebug() << "tiempo: " << tiempo;
////                }

////                j = newTME;
//                break;
//            }
//            ++j;
//        }
//    }

    qDebug() << "cleaning";
    tiemposEstimados.clear();
}
