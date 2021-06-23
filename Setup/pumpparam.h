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
    void load_data(void);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::PumpParam *ui;
};

#endif // PUMPPARAM_H
