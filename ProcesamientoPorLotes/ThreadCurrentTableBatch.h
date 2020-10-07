#ifndef THREADCURRENTTABLEBATCH_H
#define THREADCURRENTTABLEBATCH_H

#include <QThread>
#include <QList>
#include <QMutex>
#include <QWaitCondition>

#include "Batch.h"

class ThreadCurrentTableBatch : public QThread
{
    Q_OBJECT

private:
    bool pauseRequired;
    int TT; 			// We need to know our current Tiempo transcurrido to compute our new TME if we pause.
    QList<Batch*> batches;
    QMutex sync;
    QWaitCondition pauseCond;

public:
    explicit ThreadCurrentTableBatch(QThread *parent = nullptr);

    void setBatch(Batch *batch);
    void setTT(int TME);
    void pause();
    void resume();

signals:
    void updateTableCurrentBatch(Batch *batch);

protected:
    void run() override;

};

#endif // THREADCURRENTTABLEBATCH_H
