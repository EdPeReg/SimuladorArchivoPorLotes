#ifndef MEMORY_H
#define MEMORY_H

#include <QDialog>
#include <deque>

#include "Process.h"

struct Utility{
    int processID;
    int start;
    int end;
};


namespace Ui {
class Memory;
}

class Memory : public QDialog
{
    Q_OBJECT

public:
    explicit Memory(QWidget *parent = nullptr);
    ~Memory();

    void insertTable(std::deque<Process> listos, std::deque<Process> bloqueados);
    void insertRunningProcess(const Process& process);
    void insertListos(const Process& process);
    void insertBloqueado(const Process& process);
    void setTable();
    void setColor(int row, int col);
    void setProcessesLeft(int processesLeft);
    void setProcLeftCopy(int procLeftCopy);
    void setRow(int row);
    void clearRow();
    short getPagesNum(short& size);

private:
    Ui::Memory *ui;

    std::vector<Utility> utilidades;
    const short TAMANO_PAGINA = 5;
    int processesLeft;
    int procLeftCopy;
    short row;
    bool repeated;
};

#endif // MEMORY_H
