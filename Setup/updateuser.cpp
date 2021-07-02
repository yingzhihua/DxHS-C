#include "updateuser.h"
#include "ui_updateuser.h"
#include "../module/uihandler.h"
#include "../module/exglobal.h"
#include <QMessageBox>

#include "components/twobutton.h"

UpdateUser::UpdateUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateUser)
{
    ui->setupUi(this);
  //  show_data();

    ui->label_edituser_user->setGeometry(550,150,150,60);
    ui->lEdit_edituser_user->setGeometry(700,150,400,60);
    ui->label_edituser_disp_name->setGeometry(460,250,200,60);
    ui->lEdit_edituser_disp_name->setGeometry(700,250,400,60);
    ui->label_edituser_pwd->setGeometry(540,350,170,60);
    ui->lEdit_edituser_pwd->setGeometry(700,350,400,60);
    ui->label_edituser_pwd_2->setGeometry(460,450,200,60);
    ui->lEdit_edituser_pwd_2->setGeometry(700,450,400,60);
    ui->cBox_edituser_type->setGeometry(730,550,250,60);

    ui->pButton_edituser_ok->setGeometry(1180,740,300,115);
    ui->pButton_edituser_back->setGeometry(1580,740,300,115);

}

void UpdateUser::show_data()
{
    qDebug()<<"UpdateUser::show_data--getCurrIndex=="<<ExGlobal::pUserModel->getCurrIndex();
    qDebug()<<"UpdateUser::show_data--getCurName=="<<ExGlobal::pUserModel->getCurName();
    ui->lEdit_edituser_user->setText(ExGlobal::pUserModel->getCurName());
    ui->lEdit_edituser_disp_name->setText(ExGlobal::pUserModel->getCurDisplayName());
    if (ExGlobal::pUserModel->getCurUserType()==3)
        ui->cBox_edituser_type->setChecked(true);
    else
        ui->cBox_edituser_type->setChecked(false);

}

UpdateUser::~UpdateUser()
{
    delete ui;
}

void UpdateUser::showEvent(QShowEvent *event){
    Q_UNUSED(event);
   show_data();

   ui->cBox_edituser_type->setIconSize(QSize(40,40));
   ui->cBox_edituser_type->setStyleSheet("QCheckBox::indicator{width:35px;height:35px;bord-radius:7px}");

}

void UpdateUser::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}

void UpdateUser::on_pButton_edituser_back_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup_User);
}

void UpdateUser::on_pButton_edituser_ok_clicked()
{
    //QMessageBox msgBox;

    if (ui->lEdit_edituser_user->text()=="")
     {
        TwoButton::display_one_bt(tr("修改用户"),tr("用户名不能为空！"),tr("返回"));
        return;
      }
    if (ui->lEdit_edituser_pwd->text()=="")
     {
        TwoButton::display_one_bt(tr("修改用户"),tr("密码不能为空！"),tr("返回"));
        return;
      }

    if (ui->lEdit_edituser_pwd->text()!=ui->lEdit_edituser_pwd_2->text())
     {
        TwoButton::display_one_bt(tr("修改用户"),tr("密码和确认密码不相同！"),tr("返回"));
        return;
      }
    qDebug()<<"on_pButton_edituser_ok_clicked----pUserModel->index"<<ExGlobal::pUserModel->getCurrIndex();
    ExGlobal::pUserModel->updateUser(ui->lEdit_edituser_user->text(),ui->lEdit_edituser_pwd_2->text(),ui->lEdit_edituser_disp_name->text(),ui->cBox_edituser_type->checkState()?3:1);
    UIHandler::GoPage(UIHandler::PageId::Page_Setup_User);
}
