#ifndef THREADGLOBALCOUNTER_H
#define THREADGLOBALCOUNTER_H

#include <QThread>
#include <QList>

class ThreadGlobalCounter : public QThread
{
    Q_OBJECT

private:
    bool pauseRequired; // new
    int globalCounter;
    int currentIndex;
    QList<int> tiemposEstimados;
public:
    explicit ThreadGlobalCounter(QThread *parent = nullptr);

    void pause();
    void resume();
    void setTiemposEstimados(int tiempoEstimado);

protected:
    void run() override;

signals:
    void updateCounter(int n);
};

#endif // THREADGLOBALCOUNTER_H
