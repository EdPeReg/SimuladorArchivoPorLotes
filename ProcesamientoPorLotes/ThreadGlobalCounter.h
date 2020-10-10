#ifndef THREADGLOBALCOUNTER_H
#define THREADGLOBALCOUNTER_H

#include <QThread>
#include <QList>
#include <QMutex>
#include <QWaitCondition>

class ThreadGlobalCounter : public QThread
{
    Q_OBJECT

private:
    bool pauseRequired; // new
    bool keyErrorPressed;
    int globalCounter;
    int TT;
    int indexTiempo;
    QList<int> tiemposEstimados;
    QMutex sync;
    QWaitCondition pauseCond;
public:
    explicit ThreadGlobalCounter(QThread *parent = nullptr);

    void pause();
    void resume();
    void setTiemposEstimados(int tiempoEstimado);
    void setTT(int TT);
    void setKeyError(bool keyErrorPressed);
    int getTT();

protected:
    void run() override;

signals:
    void updateCounter(int n);
};

#endif // THREADGLOBALCOUNTER_H
