#ifndef THREADTIMELEFT_H
#define THREADTIMELEFT_H

#include <QThread>
#include <QList>

class ThreadTImeLeft : public QThread
{
    Q_OBJECT

private:
    QList<int> tiemposRestantes;

public:
    explicit ThreadTImeLeft(QThread *parent = nullptr);

    void setTiemposRestantes(int tiempoRestante) {
        tiemposRestantes.push_back(tiempoRestante);
    }

protected:
    void run() override;

signals:
    void updateCounter(int n);

};

#endif // THREADTIMELEFT_H
