#ifndef THREADTIMEELAPSED_H
#define THREADTIMEELAPSED_H

#include <QThread>
#include <QList>

class ThreadTImeElapsed : public QThread
{
    Q_OBJECT

private:
    bool pauseRequired;
    QList<int> tiemposEstimados;

public:
    explicit ThreadTImeElapsed(QThread *parent = nullptr);

    void setTME(int TME);

    void pause();
    void resume();
protected:
    void run() override;

signals:
    void updateCounter(int i);
};

#endif // THREADTIMEELAPSED_H
