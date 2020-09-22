#ifndef THREADGLOBALCOUNTER_H
#define THREADGLOBALCOUNTER_H

#include <QThread>
#include <QList>

class ThreadGlobalCounter : public QThread
{
    Q_OBJECT


private:
    int globalCounter;
    QList<int> tiemposEstimados;

public:
    explicit ThreadGlobalCounter(QThread *parent = nullptr);

    void setTiemposEstimados(int tiempoEstimado) {
        tiemposEstimados.push_back(tiempoEstimado);
    }

protected:
    void run() override;

signals:
    void updateCounter(int n);
};

#endif // THREADGLOBALCOUNTER_H
