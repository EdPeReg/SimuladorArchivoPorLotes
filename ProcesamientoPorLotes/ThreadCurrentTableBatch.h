#ifndef THREADCURRENTTABLEBATCH_H
#define THREADCURRENTTABLEBATCH_H

#include <QThread>
#include <QList>

#include "Batch.h"

class ThreadCurrentTableBatch : public QThread
{
    Q_OBJECT

private:
    QList<Batch*> batches;

public:
    explicit ThreadCurrentTableBatch(QThread *parent = nullptr);

    void setBatch(Batch *batch) {
        batches.push_back(batch);
    }

signals:
    void updateTableCurrentBatch(Batch *batch);

protected:
    void run() override;

};

#endif // THREADCURRENTTABLEBATCH_H
