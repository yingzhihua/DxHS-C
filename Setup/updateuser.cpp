#include "updateuser.h"
#include "ui_updateuser.h"
#include "../module/uihandler.h"
#include "../module/exglobal.h"
#include <QMessageBox>

UpdateUser::UpdateUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateUser)
{
    ui->setupUi(this);
  //  show_data();
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

   // ui->setupUi(this);

}

UpdateUser::~UpdateUser()
{
    delete ui;
}


void UpdateUser::on_pButton_edituser_back_clicked()
{
   // show_data();
     UIHandler::GoPage(UIHandler::PageId::Page_Setup_User);
}

void UpdateUser::on_pButton_edituser_ok_clicked()
{
    QMessageBox msgBox;

    if (ui->lEdit_edituser_user->text()=="")
     {
       // msgBox.setText("修改用户");
        msgBox.setInformativeText("用户名不能为空！");
        msgBox.setStandardButtons(QMessageBox::Cancel);
         msgBox.exec();
        return;
      }
    if (ui->lEdit_edituser_pwd->text()=="")
     {
       // msgBox.setText("修改用户");
        msgBox.setInformativeText("密码不能为空！");
        msgBox.setStandardButtons(QMessageBox::Cancel);
        msgBox.exec();
        return;
      }

    if (ui->lEdit_edituser_pwd->text()!=ui->lEdit_edituser_pwd_2->text())
     {
        //msgBox.setText("修改用户");
        msgBox.setInformativeText("密码和确认密码不相同！");
        msgBox.setStandardButtons(QMessageBox::Cancel);
        msgBox.exec();
        return;
      }
    qDebug()<<"on_pButton_edituser_ok_clicked----pUserModel->index"<<ExGlobal::pUserModel->getCurrIndex();
    ExGlobal::pUserModel->updateUser(ui->lEdit_edituser_user->text(),ui->lEdit_edituser_pwd_2->text(),ui->lEdit_edituser_disp_name->text(),ui->cBox_edituser_type->checkState()?3:1);
    UIHandler::GoPage(UIHandler::PageId::Page_Setup_User);
}
