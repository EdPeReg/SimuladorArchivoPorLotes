#include "ThreadProcessRunning.h"

ThreadProcessRunning::ThreadProcessRunning(QThread *parent) :
    QThread(parent)
  , timeElapsed(0)
{

}

void ThreadProcessRunning::run() {
    for(int i = 0; i < processes.size(); ++i) {
        emit updateTable(processes.at(i));

        sleep(processes.at(i)->getTiempoMaximoEst());
    }
    processes.clear();
}
