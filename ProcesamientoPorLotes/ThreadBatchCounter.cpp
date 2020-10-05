#include "ThreadBatchCounter.h"

ThreadBatchCounter::ThreadBatchCounter(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
  , indexBatch(0)
  , indexProcess(0)
  , currentBatchCounter(0)
{

}

void ThreadBatchCounter::setBatch(Batch *batch) {
    batches.push_back(batch);
}

void ThreadBatchCounter::pause() {
    sync.lock();
    pauseRequired = true;
    sync.unlock();
}

void ThreadBatchCounter::resume()
{
    sync.lock();
    pauseRequired = false;
    sync.unlock();
    pauseCond.wakeAll();
}

void ThreadBatchCounter::run()
{
//    qDebug() << "";
//    qDebug() << "batches size: " << batches.size();
//    int i = indexBatch;
//    int j = indexProcess;
//    for(; i < batches.size(); ++i) {
//        if(pauseRequired) break;
////        qDebug() << "i value: " << i;
//        int totalTME = 0;
//        QList<Process*> processes = batches.at(i)->getProcesses();
//        if(!pauseRequired) {
//            for(; j < processes.size(); ++j) {
////                qDebug() << "j value: " << j;
//                indexProcess = j;
////                qDebug() << "inside the process, index: " << indexProcess;
////                qDebug() << "PAUSADO. LOTE: " << i;
////                qDebug() << "NUMERO DE PROCESO: " << j;
//                totalTME += processes.at(j)->getTiempoMaximoEst();
//            }

//            emit updateBatchCounter(--currentBatchCounter);
//            sleep(totalTME);
//        } else {
////            qDebug() << "PAUSADOOOOOOOOOOOOOOO";
////            indexBatch = i;
////            --indexBatch; // Because first increments i and the stops, that's why i has + 1 value.
////            qDebug() << "PUASED: index batch: " << indexBatch;
////            qDebug() << "PUASED: index process: " << indexProcess;
//            break;
//        }
//    }

//    qDebug() << "index btch: " << indexBatch;
//    qDebug() << "index process: " << indexProcess;

    for(const auto& batch : batches) {
        int totalTME = 0;
        sync.lock();
        if(pauseRequired)
            pauseCond.wait(&sync);
        sync.unlock();

        for(const auto& process : batch->getProcesses()) {
            totalTME += process->getTiempoMaximoEst();
        }

        emit updateBatchCounter(--currentBatchCounter);
        sleep(totalTME);
    }

    if(!pauseRequired) {
        batches.clear();
    }
}
