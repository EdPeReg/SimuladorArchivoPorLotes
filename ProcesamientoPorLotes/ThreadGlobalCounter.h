#ifndef THREADGLOBALCOUNTER_H
#define THREADGLOBALCOUNTER_H

#include <QThread>
#include <QDebug>
#include <QVector>

class ThreadGlobalCounter : public QThread
{
    Q_OBJECT
public:
    explicit ThreadGlobalCounter(QThread *parent = nullptr);

    void setTiemposEstimados(int tiempoEstimado) {
        tiemposEstimados.push_back(tiempoEstimado);
    }

private:
    int tiempoMaxEst;
    int sum;
    QVector<int> tiemposEstimados;

protected:
    void run();

signals:
    void updateCounter(int n);
};

#endif // THREADGLOBALCOUNTER_H
