#ifndef PIDPARAM_H
#define PIDPARAM_H

#include <QDialog>

namespace Ui {
class PIDParam;
}

class PIDParam : public QDialog
{
    Q_OBJECT

public:
    explicit PIDParam(QWidget *parent = nullptr);
    ~PIDParam();
    void show_data(void);

private slots:
    void on_PIDParam_accepted();

private:
    Ui::PIDParam *ui;
};

#endif // PIDPARAM_H
