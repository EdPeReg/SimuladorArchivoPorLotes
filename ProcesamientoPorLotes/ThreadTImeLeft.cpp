#include "ThreadTImeLeft.h"

ThreadTImeLeft::ThreadTImeLeft(QThread *parent) : QThread(parent)
{

}

void ThreadTImeLeft::run()
{
    for(const auto& tiempoRestante : tiemposRestantes) {
        int counter = tiempoRestante;
        for(int i = 0; i < tiempoRestante; ++i) {
            emit updateCounter(counter--);
            sleep(1);
        }

    }
    tiemposRestantes.clear();
}
