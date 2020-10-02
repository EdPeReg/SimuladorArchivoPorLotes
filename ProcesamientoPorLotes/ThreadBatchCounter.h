#ifndef THREADBATCHCOUNTER_H
#define THREADBATCHCOUNTER_H

#include <QThread>
#include <QList>

#include "Batch.h"

class ThreadBatchCounter : public QThread
{
    Q_OBJECT

private:
    bool pauseRequired;
    QList<Batch *> batches;
public:
    explicit ThreadBatchCounter(QThread *parent = nullptr);

    int currentBatchCounter;

    void setBatch(Batch *batch);
    void pause();
    void resume();

protected:
    void run() override;

signals:
    void updateBatchCounter(int n);
};

#endif // THREADBATCHCOUNTER_H
