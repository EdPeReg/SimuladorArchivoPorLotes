#ifndef THREADTABLEFINISH_H
#define THREADTABLEFINISH_H

#include <QThread>
#include <QList>
#include <QMutex>
#include <QWaitCondition>

#include "Batch.h"

class ThreadTableFinish : public QThread
{
    Q_OBJECT

private:
    bool pauseRequired;
    QList<Batch*> batches;
    QMutex sync;
    QWaitCondition pauseCond;
public:
    explicit ThreadTableFinish(QThread *parent = nullptr);

    void setBatch(Batch* b);
    void pause();
    void resume();

signals:
    void updateTableFinish(Process* process);
    void reset();

protected:
    void run() override;
};

#endif // THREADTABLEFINISH_H
