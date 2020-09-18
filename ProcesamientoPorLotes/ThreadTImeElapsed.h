#ifndef THREADTIMEELAPSED_H
#define THREADTIMEELAPSED_H

#include <QThread>
#include <QVector>

class ThreadTImeElapsed : public QThread
{
    Q_OBJECT

private:
    QVector<int> tiemposEstimados;

public:
    explicit ThreadTImeElapsed(QThread *parent = nullptr);

    void setTME(int TME) {
        tiemposEstimados.push_back(TME);
    }

protected:
    void run() override;

signals:
    void updateCounter(int i);
};

#endif // THREADTIMEELAPSED_H
