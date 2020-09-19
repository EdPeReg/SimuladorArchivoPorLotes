#include "ThreadFinish.h"

ThreadFinish::ThreadFinish(QThread *parent) :
    QThread(parent)
  , finish(false)
{

}

void ThreadFinish::run() {
    if(!finish) {
        for(const auto& process : processedFinished) {
            emit updateTable(process);
            sleep(process->getTiempoMaximoEst());
        }
    }
    processedFinished.clear();

}
