#include "ThreadProcessRunning.h"

ThreadProcessRunning::ThreadProcessRunning(QThread *parent) :
    QThread(parent)
  , finish(false)
{

}

void ThreadProcessRunning::run() {
    if(!finish) {
        for(int i = 0; i < processes.size(); ++i) {
            emit updateTable(processes.at(i));
            sleep(processes.at(i)->getTiempoMaximoEst());
        }
    }

    processes.clear();
}
