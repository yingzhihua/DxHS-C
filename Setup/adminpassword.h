#ifndef ADMINPASSWORD_H
#define ADMINPASSWORD_H

#include <QDialog>

namespace Ui {
class AdminPassword;
}

class AdminPassword : public QDialog
{
    Q_OBJECT

public:
    explicit AdminPassword(QWidget *parent = nullptr);
    ~AdminPassword();

private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_Cencel_clicked();

private:
    Ui::AdminPassword *ui;
};

#endif // ADMINPASSWORD_H
