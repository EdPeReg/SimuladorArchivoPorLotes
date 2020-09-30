#ifndef THREADPROCESSRUNNING_H
#define THREADPROCESSRUNNING_H

#include <QThread>
#include <QList>

#include "Batch.h"

class ThreadTables : public QThread
{
    Q_OBJECT
private:
    bool stop;
    QList<Batch*> batches;

public:
    explicit ThreadTables(QThread *parent = nullptr);

    void setBatch(Batch *batch) {
        batches.push_back(batch);
    }

    void setStop(bool stop) { // new
        this->stop = stop;
    }

    bool getStop() const { // new
        return stop;
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
