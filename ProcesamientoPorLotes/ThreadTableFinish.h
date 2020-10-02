#ifndef THREADTABLEFINISH_H
#define THREADTABLEFINISH_H

#include <QThread>
#include <QList>

#include "Batch.h"

class ThreadTableFinish : public QThread
{
    Q_OBJECT

private:
    QList<Batch*> batches;

public:
    explicit ThreadTableFinish(QThread *parent = nullptr);

    void setBatch(Batch* b) {
        batches.push_back(b);
    }

signals:
    void updateTableFinish(Process* process);
    void reset();

protected:
    void run() override;
};

#endif // THREADTABLEFINISH_H
