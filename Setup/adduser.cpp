#include "adduser.h"
#include "ui_adduser.h"
#include "../module/uihandler.h"
#include "../module/exglobal.h"
#include "components/twobutton.h"

AddUser::AddUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);
    //show_data();

    ui->label_background->setGeometry(566,180,811,485);

    ui->label_adduser_user->setGeometry(749,272,80,40);
    ui->lEdit_adduser_user->setGeometry(848,258,404,64);
    ui->label_adduser_disp_name->setGeometry(712,353,150,40);
    ui->lEdit_adduser_disp_name->setGeometry(848,339,404,64);
    ui->label_adduser_pwd->setGeometry(749,434,80,40);
    ui->lEdit_adduser_pwd->setGeometry(848,420,404,64);
    ui->label_adduser_pwd_2->setGeometry(658,514,170,40);
    ui->lEdit_adduser_pwd_2->setGeometry(848,500,404,64);
    ui->cBox_adduser_type->setGeometry(850,581,250,40);


    ui->pushButton->setGeometry(1450,624,299,106);
    ui->pushButton_2->setGeometry(1450,754,299,106);

}

void AddUser::show_data()
{

}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::showEvent(QShowEvent *event){
    Q_UNUSED(event);
   show_data();

   ui->cBox_adduser_type->setIconSize(QSize(40,40));
   ui->cBox_adduser_type->setStyleSheet("QCheckBox::indicator{width:35px;height:35px;bord-radius:7px}");

   ui->lEdit_adduser_user->setText("");
   ui->lEdit_adduser_disp_name->setText("");
   ui->lEdit_adduser_pwd->setText("");
   ui->lEdit_adduser_pwd_2->setText("");
    ui->cBox_adduser_type->setChecked(false);

}

void AddUser::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}

void AddUser::on_pushButton_clicked()
{
    if (ui->lEdit_adduser_user->text()=="")
     {
        TwoButton::display_one_bt(tr("增加用户"),tr("用户名不能为空！"),tr("返回"));
        return;
      }
    if (ui->lEdit_adduser_pwd->text()=="")
     {
        TwoButton::display_one_bt(tr("增加用户"),tr("密码不能为空！"),tr("返回"));
        return;
      }

    if (ui->lEdit_adduser_pwd->text()!=ui->lEdit_adduser_pwd_2->text())
     {
        TwoButton::display_one_bt(tr("增加用户"),tr("密码和确认密码不相同！"),tr("返回"));
        return;
      }
    ExGlobal::pUserModel->addUser( ui->lEdit_adduser_user->text(),ui->lEdit_adduser_pwd_2->text(),ui->lEdit_adduser_disp_name->text(),ui->cBox_adduser_type->checkState()?3:1);
    UIHandler::GoPage(UIHandler::PageId::Page_Setup_User);
}

void AddUser::on_pushButton_2_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup_User);
}
