#include "ThreadTImeLeft.h"

ThreadTImeLeft::ThreadTImeLeft(QThread *parent) :
    QThread(parent)
  , stop(false)
{

}

void ThreadTImeLeft::run()
{
    stop = false;
    for(const auto& tiempoRestante : tiemposRestantes) {
        int counter = tiempoRestante;
        for(int i = 0; i < tiempoRestante; ++i) {
            if(!stop) {
                emit updateCounter(counter--);
                sleep(1);
            } else {
                break;
            }
        }
    }
    tiemposRestantes.clear();
}
