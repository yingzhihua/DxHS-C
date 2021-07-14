#include "adminpassword.h"
#include "ui_adminpassword.h"
#include "../module/uihandler.h"
#include "../module/exglobal.h"
#include <QMessageBox>
#include "components/twobutton.h"

AdminPassword::AdminPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminPassword)
{
    ui->setupUi(this);
    userModel= new UserModel();


    int init_x = (ExGlobal::contentWidth-810)/2;
    int init_y = (ExGlobal::contentHeight-565)/2+150;
    setGeometry(init_x,init_y,810,565);

    ui->label_background->setGeometry(561,255,798,375);

    ui->label_oldpwd->setGeometry(723,346,123,42);
    ui->lineEdit_oldpwd->setGeometry(874,333,404,64);
    ui->label_newpwd->setGeometry(723,433,123,42);
    ui->lineEdit_newpwd->setGeometry(874,420,404,64);
    ui->label_newpwd_2->setGeometry(641,520,207,42);
    ui->lineEdit_newpwd_2->setGeometry(874,507,404,64);
    ui->pushButton_OK->setGeometry(1450,624,299,106);
    ui->pushButton_Cencel->setGeometry(1450,754,299,106);

 }

AdminPassword::~AdminPassword()
{
    delete ui;
}

void AdminPassword::on_pushButton_OK_clicked()
{
    if (ui->lineEdit_oldpwd->text()=="")
     {
        TwoButton::display_one_bt(tr("修改密码"),tr("密码不能为空！"),tr("返回"));
        return;
      }
    if (ui->lineEdit_newpwd->text()=="")
     {
        TwoButton::display_one_bt(tr("修改密码"),tr("新密码不能为空！"),tr("返回"));
        return;
      }

    if (ui->lineEdit_newpwd->text()!=ui->lineEdit_newpwd_2->text())
     {
        TwoButton::display_one_bt(tr("修改密码"),tr("新密码和确认密码不相同！"),tr("返回"));
        return;
      }
     userModel->updatePassword(ui->lineEdit_oldpwd->text(),ui->lineEdit_newpwd->text());
     UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}

void AdminPassword::on_pushButton_Cencel_clicked()
{
      UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
