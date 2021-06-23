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

public:
    explicit User(QWidget *parent = nullptr);
    ~User();
    void load_data(void);


private slots:
    void on_pButton_user_del_clicked();

    void on_pButton_user_add_clicked();

    void on_listWidget_2_currentRowChanged(int currentRow);

    void on_pButton_user_edit_clicked();

private:
    Ui::User *ui;
};

#endif // USER_H
