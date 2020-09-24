#ifndef THREADPROCESSRUNNING_H
#define THREADPROCESSRUNNING_H

#include <QThread>
#include <QList>
#include <QDebug>

#include "Batch.h"

class ThreadProcessRunning : public QThread
{
    Q_OBJECT
private:
    QList<Batch*> batches;

public:
    explicit ThreadProcessRunning(QThread *parent = nullptr);

    void setBatch(Batch *batch) {
        batches.push_back(batch);
    }

protected:
    void run() override;

signals:
    void updateTableProcessRunning(Process *process);
    void updateTableFinish(Process *process);
    void updateTableCurrentBatch(Batch* batch);
    void reset();
};

#endif // THREADPROCESSRUNNING_H
