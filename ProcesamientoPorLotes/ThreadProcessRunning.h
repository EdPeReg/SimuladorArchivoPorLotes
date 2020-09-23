#ifndef THREADPROCESSRUNNING_H
#define THREADPROCESSRUNNING_H

#include <QThread>
#include <QList>
#include <QDebug>

#include "Process.h"

class ThreadProcessRunning : public QThread
{
    Q_OBJECT
private:
    QList<Process*> processes;

public:
    explicit ThreadProcessRunning(QThread *parent = nullptr);

    bool finish;

    void setProcess(Process* process) {
        processes.push_back(process);
    }

protected:
    void run() override;

signals:
    void updateTable(Process *process);
    void updateTableFinish(Process *process);
    void reset();
};

#endif // THREADPROCESSRUNNING_H
