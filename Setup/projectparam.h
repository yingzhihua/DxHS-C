#ifndef PROJECTPARAM_H
#define PROJECTPARAM_H

#include <QDialog>

namespace Ui {
class ProjectParam;
}

class ProjectParam : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectParam(QWidget *parent = nullptr);
    ~ProjectParam();

    void load_data(void);

private slots:
    void on_pButton_machine_back_clicked();

    void on_rButton_c1_clicked();

    void on_rButton_c2_clicked();

    void on_rButton_c3_clicked();

    void on_rButton_c4_clicked();

    void on_rButton_c5_clicked();

    void on_rButton_b1_clicked();

    void on_rButton_b2_clicked();

    void on_spinBox_high_valueChanged(int arg1);

    void on_spinBox_pre_valueChanged(const QString &arg1);

    void on_spinBox_pre_valueChanged(int arg1);

private:
    Ui::ProjectParam *ui;
};

#endif // PROJECTPARAM_H
