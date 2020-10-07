#include "ThreadCurrentTableBatch.h"

ThreadCurrentTableBatch::ThreadCurrentTableBatch(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
{

}

void ThreadCurrentTableBatch::setBatch(Batch *batch) {
    batches.push_back(batch);
}

void ThreadCurrentTableBatch::setTT(int TT)
{
    this->TT = TT;
}

void ThreadCurrentTableBatch::pause() {
    sync.lock();
    pauseRequired = true;
    sync.unlock();
}

void ThreadCurrentTableBatch::resume() {
    sync.lock();
    pauseRequired = false;
    sync.unlock();
    pauseCond.wakeAll();
}

void ThreadCurrentTableBatch::run()
{
//    bool hitPause = false;
//    int indexBatch = 0;
//    int indexProcess = 0;
//    while(indexBatch < batches.size()) {
//        int newTME = 0;
//        Batch *batch = batches.at(indexBatch);
//        QList<Process *> processes = batch->getProcesses();

//        for(const auto& process : processes) {
//            qDebug() << "process TME: " << process->getTiempoMaximoEst();
//        }

//        // To avoid to decrement immediately once we continue.
//        if(!hitPause) {
//            emit updateTableCurrentBatch(batch); // FIX THIS
//        }

//        hitPause = false;
//        while(indexProcess < processes.size()) {
//            sleep(processes.at(indexProcess)->getTiempoMaximoEst());

//            sync.lock();
//            if(pauseRequired) {
//                hitPause = true;
//                newTME = processes.at(indexProcess)->getTiempoMaximoEst() - TT;
//                qDebug() << "new TME: " << newTME;

//                // Replace in that process its new TME.
//                processes.at(indexProcess)->setTiempoMaximoEst(newTME);
//                pauseCond.wait(&sync);

//                // Decrement because yes or yes increases after the pause.
//                --indexBatch;
////                sleep(newTME);
//            }
//            sync.unlock();

//            if(hitPause) break;
//            ++indexProcess;
//        }
//        ++indexBatch;

//        // If we didn't press the pause key, we will start again.
//        if(!hitPause) {
//            indexProcess = 0;
//        }
//    }

    for(const auto& batch : batches) {
        emit updateTableCurrentBatch(batch);
        for(const auto& process : batch->getProcesses()) {
            sleep(process->getTiempoMaximoEst());

            sync.lock();
            if(pauseRequired) {
                int newTME = process->getTiempoMaximoEst() - TT;
                pauseCond.wait(&sync);
                sleep(newTME);
            }
            sync.unlock();
        }
    }

    batches.clear();
}
