#ifndef USER_H
#define USER_H

#include <QDialog>
#include "../module/dao/usermodel.h"
#include "updateuser.h"

namespace Ui {
class User;
}

class User : public QDialog
{
    Q_OBJECT

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);


public:
    explicit User(QWidget *parent = nullptr);
    ~User();
    void load_data(void);
    int currIndex=0;

    void mode_data();


private slots:
    void on_pButton_user_del_clicked();

    void on_pButton_user_add_clicked();

    void on_listWidget_2_currentRowChanged(int currentRow);

    void on_pButton_user_edit_clicked();

    void on_pushButton_canel_clicked();

    void on_pushButton_user_return_clicked();

    void on_pButton_user_return_clicked();

    void deleteUserBack();


    void on_pushButton_back_clicked();

private:
    Ui::User *ui;
};

#endif // USER_H
