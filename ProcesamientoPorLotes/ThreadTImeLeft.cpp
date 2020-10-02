#include "ThreadTImeLeft.h"

ThreadTImeLeft::ThreadTImeLeft(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
{

}

void ThreadTImeLeft::setTiemposRestantes(int tiempoRestante) {
    tiemposRestantes.push_back(tiempoRestante);
}

void ThreadTImeLeft::pause() {
    pauseRequired = true;
}

void ThreadTImeLeft::resume() {
    pauseRequired = false;
}

void ThreadTImeLeft::run()
{
    for(const auto& tiempoRestante : tiemposRestantes) {
        int counter = tiempoRestante;
        for(int i = 0; i < tiempoRestante; ++i) {
            if(!pauseRequired) {
                emit updateCounter(counter--);
                sleep(1);
            } else {
                break;
            }
        }
        if(pauseRequired) break;
    }

    if(!pauseRequired) {
        tiemposRestantes.clear();
    }
}
