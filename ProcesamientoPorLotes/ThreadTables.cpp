#include "ThreadTables.h"

ThreadTables::ThreadTables(QThread *parent) :
    QThread(parent)
    , stop(false)
{

}

void ThreadTables::run() {
    stop = false;
    for(const auto& batch : batches) {
//        qDebug() << "stoppppp: " << stop;

        emit updateTableCurrentBatch(batch);
        for(const auto& process : batch->getProcesses()) {
//            qDebug() << "dentroooooooooooo";
            emit updateTableProcessRunning(process);
            sleep(process->getTiempoMaximoEst());
            emit updateTableFinish(process);
        }
    }

    emit reset();
    batches.clear();
}
