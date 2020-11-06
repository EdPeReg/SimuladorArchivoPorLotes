#ifndef PROCESSESDIALOG_H
#define PROCESSESDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QDebug>

#include <vector>

#include "Process.h"

namespace Ui {
class ProcessesDialog;
}

enum PROCESS_INFO {
    ID_PI, NAME_PI, OPERATION_PI, RESULT_PI, ESTADO_PI, TME_PI,
    TL_PI, TF_PI, TR_PI, TRE_PI, TE_PI, TS_PI, TRCPU_PI
};

class ProcessesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessesDialog(QWidget *parent = nullptr);
    ~ProcessesDialog();

    void setProcesses(const std::vector<Process>& proesses);

protected:
    void keyPressEvent(QKeyEvent * e) override;

private:
    Ui::ProcessesDialog *ui;
};

#endif // PROCESSESDIALOG_H
