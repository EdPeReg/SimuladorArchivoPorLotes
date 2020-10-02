#ifndef THREADTABLERUNNING_H
#define THREADTABLERUNNING_H

#include <QThread>
#include <QList>

#include "Batch.h"

class ThreadTableRunning : public QThread
{
    Q_OBJECT

private:
    QList<Batch*> batches;

public:
    explicit ThreadTableRunning(QThread *parent = nullptr);

    void setBatch(Batch *batch) {
        batches.push_back(batch);
    }

signals:
    void updateTableProcessRunning(Process* process);

protected:
    void run() override;



};

#endif // THREADTABLERUNNING_H
