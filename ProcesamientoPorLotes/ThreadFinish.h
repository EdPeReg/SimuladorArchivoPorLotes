#ifndef THREADFINISH_H
#define THREADFINISH_H

#include <QThread>
#include <QVector>

#include "Process.h"

class ThreadFinish : public QThread
{
    Q_OBJECT

private:
    QVector<Process*> processedFinished;

protected:
    void run() override;

public:
    explicit ThreadFinish(QThread *parent = nullptr);

    bool finish;

    void setProcessFinished(Process* proccess) {
        processedFinished.push_back(proccess);
    }

signals:
    void updateTable(Process* process);
};

#endif // THREADFINISH_H
