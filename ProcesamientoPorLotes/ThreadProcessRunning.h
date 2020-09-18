#ifndef THREADPROCESSRUNNING_H
#define THREADPROCESSRUNNING_H

#include <QThread>
#include <QVector>

#include "Process.h"

class ThreadProcessRunning : public QThread
{
    Q_OBJECT
private:
    int timeElapsed;
    QVector<Process*> processes;

public:
    explicit ThreadProcessRunning(QThread *parent = nullptr);

    int gettimeElapsed() const {
        return timeElapsed;
    }

    void setProcess(Process* process) {
        processes.push_back(process);
    }

protected:
    void run() override;

signals:
    void updateTable(Process *process);
};

#endif // THREADPROCESSRUNNING_H
