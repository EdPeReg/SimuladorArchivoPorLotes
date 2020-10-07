#include "ThreadBatchCounter.h"

ThreadBatchCounter::ThreadBatchCounter(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
  , pauseHit(false)
  , indexBatch(0)
  , indexProcess(0)
  , TT(0)
  , currentBatchCounter(0)
{

}

void ThreadBatchCounter::setBatch(Batch *batch) {
    batches.push_back(batch);
}

void ThreadBatchCounter::setTT(int TT)
{
    this->TT = TT;
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
    qDebug() << "";
    int newTME = 0;
//    int i = 0;

//    while(i < batches.size()) {
//        qDebug() << "i: " << i;
//        qDebug() << "current batch counter: " << currentBatchCounter;


//        Batch *batch = batches.at(i);
//        QList<Process *> processes = batch->getProcesses();
//        int totalTME = 0;
//        for(const auto& process : processes) {
//            totalTME += process->getTiempoMaximoEst();
//        }

//        emit updateBatchCounter(--currentBatchCounter);
//        sleep(totalTME); // SE QUEDA AQUI DORMIDO, AUNQUE PAUSE SEA VERDADERO/
//        ++i;
//    }

    bool isNewTME = false;
    while(indexBatch < batches.size()) {
        qDebug() << "index batch: " << indexBatch;

        // To avoid to decrement immediately once we continue.
        if(!pauseHit) {
            emit updateBatchCounter(--currentBatchCounter); // FIX THIS
        }

        pauseHit = false;
        Batch *batch = batches.at(indexBatch);
        QList<Process *> processes = batch->getProcesses();

//        for(const auto& process : processes) {
//            qDebug() << process->getTiempoMaximoEst();
//        }

        qDebug() << "";
        while(indexProcess < processes.size()) {
//            qDebug() << "index process value: " << indexProcess;
            sleep(processes.at(indexProcess)->getTiempoMaximoEst());

            sync.lock();
            if(pauseRequired) {
                pauseHit = true;
                isNewTME = true;
                newTME = processes.at(indexProcess)->getTiempoMaximoEst() - TT;

                // Replace in that process, its new TME.
                processes.at(indexProcess)->setTiempoMaximoEst(newTME);
//                qDebug() << "paused at batch index: " << indexBatch;
//                qDebug() << "paused at process index: " << indexProcess;
//                qDebug() << "Tiempo transcurrido" << TT;
//                qDebug() << "new TME" << newTME;

                pauseCond.wait(&sync);

                // Decrement because yes or yes increases after the pause.
                --indexBatch;
            }
            sync.unlock();

            if(pauseHit) break;
            ++indexProcess;
        }
        ++indexBatch;

        // If we didn't press the pause key, we will start again.
        if(!pauseHit) {
            indexProcess = 0;
        }
    }

//    for(const auto& batch : batches) {
//        int totalTME = 0;

//        for(const auto& process : batch->getProcesses()) {
//            totalTME += process->getTiempoMaximoEst();
//        }

//        emit updateBatchCounter(--currentBatchCounter);
//        sleep(totalTME);

//        sync.lock();
//        if(pauseRequired) {
//            qDebug() << "pausado";
//            pauseCond.wait(&sync);
//        }
//        sync.unlock();

//        ++i;
//    }
    batches.clear();
}
