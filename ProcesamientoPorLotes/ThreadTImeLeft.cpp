#include "ThreadTImeLeft.h"

#include <QDebug>

ThreadTImeLeft::ThreadTImeLeft(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
{

}

void ThreadTImeLeft::setTiemposRestantes(int tiempoRestante) {
    tiemposRestantes.push_back(tiempoRestante);
}

void ThreadTImeLeft::pause() {
    sync.lock();
    pauseRequired = true;
    sync.unlock();
}

void ThreadTImeLeft::resume() {
    sync.lock();
    pauseRequired = false;
    sync.unlock();
    pauseCond.wakeAll();
}

void ThreadTImeLeft::run()
{
    for(const auto& tiempoRestante : tiemposRestantes) {
        int counter = tiempoRestante;
        for(int i = 0; i < tiempoRestante; ++i) {
            sync.lock();
            if(pauseRequired) {
                pauseCond.wait(&sync);
            }
            sync.unlock();

            emit updateCounter(counter--);
            sleep(1);
        }
    }

    tiemposRestantes.clear();
}
