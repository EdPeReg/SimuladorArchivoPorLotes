#ifndef THREADCURRENTTABLEBATCH_H
#define THREADCURRENTTABLEBATCH_H

#include <QThread>
#include <QList>

#include "Batch.h"

class ThreadCurrentTableBatch : public QThread
{
    Q_OBJECT

private:
    bool pauseRequired;
    QList<Batch*> batches;

public:
    explicit ThreadCurrentTableBatch(QThread *parent = nullptr);

    void setBatch(Batch *batch);
    void pause();
    void resume();

signals:
    void updateTableCurrentBatch(Batch *batch);

protected:
    void run() override;

};

#endif // THREADCURRENTTABLEBATCH_H
