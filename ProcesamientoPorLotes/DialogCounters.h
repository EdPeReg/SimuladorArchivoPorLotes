#ifndef DIALOGCOUNTERS_H
#define DIALOGCOUNTERS_H

#include <QDialog>

#include "ThreadGlobalCounter.h"

namespace Ui {
class DialogCounters;
}

class DialogCounters : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCounters(QWidget *parent = nullptr);
    ~DialogCounters();

private:
    Ui::DialogCounters *ui;

    ThreadGlobalCounter *threadGlobalCounter;

    void updateGlobalCounter(int n);
};

#endif // DIALOGCOUNTERS_H
