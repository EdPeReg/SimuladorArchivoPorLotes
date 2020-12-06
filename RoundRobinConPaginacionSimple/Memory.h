#ifndef MEMORY_H
#define MEMORY_H

#include <QDialog>

namespace Ui {
class Memory;
}

class Memory : public QDialog
{
    Q_OBJECT

public:
    explicit Memory(QWidget *parent = nullptr);
    ~Memory();

private:
    Ui::Memory *ui;
};

#endif // MEMORY_H
