#ifndef THREADTIMELEFT_H
#define THREADTIMELEFT_H

#include <QThread>
#include <QList>

class ThreadTImeLeft : public QThread
{
    Q_OBJECT

private:
    bool stop;
    QList<int> tiemposRestantes;

public:
    explicit ThreadTImeLeft(QThread *parent = nullptr);

    void setTiemposRestantes(int tiempoRestante) {
        tiemposRestantes.push_back(tiempoRestante);
    }

    void setStop(bool stop) {
        this->stop = stop;
    }
protected:
    void run() override;

signals:
    void updateCounter(int n);

};

#endif // THREADTIMELEFT_H
