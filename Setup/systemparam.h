#ifndef SYSTEMPARAM_H
#define SYSTEMPARAM_H

#include <QDialog>

namespace Ui {
class SystemParam;
}

class SystemParam : public QDialog
{
    Q_OBJECT

public:
    explicit SystemParam(QWidget *parent = nullptr);
    ~SystemParam();

private:
    Ui::SystemParam *ui;
};

#endif // SYSTEMPARAM_H
