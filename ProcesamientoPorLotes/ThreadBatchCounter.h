#ifndef THREADBATCHCOUNTER_H
#define THREADBATCHCOUNTER_H

#include <QThread>
#include <QList>

#include "Batch.h"

class ThreadBatchCounter : public QThread
{
    Q_OBJECT

private:
    QList<Batch *> batches;

public:
    explicit ThreadBatchCounter(QThread *parent = nullptr);

    int currentBatchCounter;

    void setBatch(Batch *batch) {
        batches.push_back(batch);
    }

protected:
    void run() override;

signals:
    void updateBatchCounter(int n);
};

#endif // THREADBATCHCOUNTER_H
