#ifndef THREADGLOBALCOUNTER_H
#define THREADGLOBALCOUNTER_H

#include <QThread>
#include <QDebug>

class ThreadGlobalCounter : public QThread
{
    Q_OBJECT
public:
    explicit ThreadGlobalCounter(QThread *parent = nullptr);

private:

protected:
    void run();

signals:
    void updateGlobalCounter();
};

#endif // THREADGLOBALCOUNTER_H
