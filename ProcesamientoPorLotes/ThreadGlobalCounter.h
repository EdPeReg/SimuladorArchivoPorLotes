#ifndef THREADGLOBALCOUNTER_H
#define THREADGLOBALCOUNTER_H

#include <QThread>
#include <QList>
#include <QMutex>

class ThreadGlobalCounter : public QThread
{
    Q_OBJECT

private:
    bool stop; // new
    int globalCounter;
    int currentIndex;
    QList<int> tiemposEstimados;
//    QMutex mutex; // new

public:
    explicit ThreadGlobalCounter(QThread *parent = nullptr);

    void setStop(bool stop) { // new
        this->stop = stop;
    }

    void setTiemposEstimados(int tiempoEstimado) {
        tiemposEstimados.push_back(tiempoEstimado);
    }

protected:
    void run() override;

signals:
    void updateCounter(int n);
};

#endif // THREADGLOBALCOUNTER_H
