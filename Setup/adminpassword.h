#ifndef ADMINPASSWORD_H
#define ADMINPASSWORD_H

#include <QDialog>


#include "../module/dao/usermodel.h"

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
    UserModel *userModel;
};

#endif // ADMINPASSWORD_H
