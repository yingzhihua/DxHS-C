#ifndef UPDATEUSER_H
#define UPDATEUSER_H

#include <QDialog>

namespace Ui {
class UpdateUser;
}

class UpdateUser : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateUser(QWidget *parent = nullptr);
    void show_data(void);
    ~UpdateUser();


private slots:
    void on_pButton_edituser_back_clicked();

    void on_pButton_edituser_ok_clicked();

private:
    Ui::UpdateUser *ui;
};

#endif // UPDATEUSER_H
