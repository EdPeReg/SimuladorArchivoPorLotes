#ifndef THREADGLOBALCOUNTER_H
#define THREADGLOBALCOUNTER_H

#include <QThread>
#include <QVector>

class ThreadGlobalCounter : public QThread
{
    Q_OBJECT


private:
    int globalCounter;
    QVector<int> tiemposEstimados;

public:
    explicit ThreadGlobalCounter(QThread *parent = nullptr);

    void setTiemposEstimados(int tiempoEstimado) {
        tiemposEstimados.push_back(tiempoEstimado);
    }


public:
    void setContadorGlobal(int contadorGlobal) {
        tiemposEstimados.push_back(contadorGlobal);
    }

protected:
    void run() override;

signals:
    void updateCounter(int n);
};

#endif // THREADGLOBALCOUNTER_H
