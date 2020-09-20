#ifndef THREADBATCHCOUNTER_H
#define THREADBATCHCOUNTER_H

#include <QThread>
#include <QVector>

#include "Batch.h"

class ThreadBatchCounter : public QThread
{
    Q_OBJECT

private:
    QVector<Batch *> batches;

    void clean() {
        for(auto& batch : batches) {
            delete batch;
        }
    }

public:
    explicit ThreadBatchCounter(QThread *parent = nullptr);

    int currentBatchCounter;
    bool finish;

    void setBatch(Batch *batch) {
        batches.push_back(batch);
    }

protected:
    void run() override;

signals:
    void updateBatchCounter(int n);
};

#endif // THREADBATCHCOUNTER_H
