#ifndef THREADPROCESSRUNNING_H
#define THREADPROCESSRUNNING_H

#include <QThread>
#include <QList>
#include <QDebug>

#include "Batch.h"
#include "Process.h"

class ThreadProcessRunning : public QThread
{
    Q_OBJECT
private:
    QList<Batch*> batches;
    QList<Process*> processes;

public:
    explicit ThreadProcessRunning(QThread *parent = nullptr);

    void setBatch(Batch *batch) {
        batches.push_back(batch);
    }

    void setProcess(Process* process) {
        processes.push_back(process);
    }

protected:
    void run() override;

signals:
    void updateTable(Process *process);
    void updateTableFinish(Process *process);
    void updateTableCurrentBatch(Batch* batch); // new
    void reset();
};

#endif // THREADPROCESSRUNNING_H
