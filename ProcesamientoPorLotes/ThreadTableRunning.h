#ifndef THREADTABLERUNNING_H
#define THREADTABLERUNNING_H

#include <QThread>
#include <QList>
#include <QMutex>
#include <QWaitCondition>

#include "Batch.h"

class ThreadTableRunning : public QThread
{
    Q_OBJECT

private:
    bool pauseRequired;
    QList<Batch*> batches;
    QMutex sync;
    QWaitCondition pauseCond;

public:
    explicit ThreadTableRunning(QThread *parent = nullptr);

    void setBatch(Batch *batch);
    void pause();
    void resume();

signals:
    void updateTableProcessRunning(Process* process);

protected:
    void run() override;
};

#endif // THREADTABLERUNNING_H
