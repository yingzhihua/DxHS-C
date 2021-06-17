#ifndef PUMPPARAM_H
#define PUMPPARAM_H

#include <QDialog>

namespace Ui {
class PumpParam;
}

class PumpParam : public QDialog
{
    Q_OBJECT

public:
    explicit PumpParam(QWidget *parent = nullptr);
    ~PumpParam();

private:
    Ui::PumpParam *ui;
};

#endif // PUMPPARAM_H
