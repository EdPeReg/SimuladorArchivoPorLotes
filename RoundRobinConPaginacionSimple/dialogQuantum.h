#ifndef DIALOGQUANTUM_H
#define DIALOGQUANTUM_H

#include <QDialog>

namespace Ui {
class dialogQuantum;
}

class dialogQuantum : public QDialog
{
    Q_OBJECT

public:
    explicit dialogQuantum(QWidget *parent = nullptr);
    ~dialogQuantum();

    int getQuantum() const;

private:
    Ui::dialogQuantum *ui;
};

#endif // DIALOGQUANTUM_H
