#ifndef THREADPROCESSRUNNING_H
#define THREADPROCESSRUNNING_H

#include <QThread>
#include <QVector>
#include <QDebug>

#include "Process.h"

class ThreadProcessRunning : public QThread
{
    Q_OBJECT
private:
    QVector<Process*> processes;

public:
    explicit ThreadProcessRunning(QThread *parent = nullptr);

    bool finish;
    int counter = 0;

    void setProcess(Process* process) {
        processes.push_back(process);
    }

protected:
    void run() override;

signals:
    void updateTable(Process *process);
    void reset();
};

#endif // THREADPROCESSRUNNING_H
