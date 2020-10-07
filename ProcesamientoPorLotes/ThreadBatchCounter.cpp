#include "ThreadBatchCounter.h"

ThreadBatchCounter::ThreadBatchCounter(QThread *parent) :
    QThread(parent)
  , pauseRequired(false)
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

// If we hit pause key, we will compute its new TME from that process, we do this
// to wait the correct time.
void ThreadBatchCounter::run()
{
    bool hitPause = false;
    int newTME = 0;
    int indexBatch = 0;
    int indexProcess = 0;
    while(indexBatch < batches.size()) {
        // To avoid to decrement immediately once we continue.
        if(!hitPause) {
            emit updateBatchCounter(--currentBatchCounter); // FIX THIS
        }

        hitPause = false;
        Batch *batch = batches.at(indexBatch);
        QList<Process *> processes = batch->getProcesses();

        while(indexProcess < processes.size()) {
            sleep(processes.at(indexProcess)->getTiempoMaximoEst());

            sync.lock();
            if(pauseRequired) {
                hitPause = true;
                newTME = processes.at(indexProcess)->getTiempoMaximoEst() - TT;

                // Replace in that process its new TME.
                processes.at(indexProcess)->setTiempoMaximoEst(newTME);
                pauseCond.wait(&sync);

                // Decrement because yes or yes increases after the pause.
                --indexBatch;
            }
            sync.unlock();

            if(hitPause) break;
            ++indexProcess;
        }
        ++indexBatch;

        // If we didn't press the pause key, we will start again.
        if(!hitPause) {
            indexProcess = 0;
        }
    }
    batches.clear();
}
