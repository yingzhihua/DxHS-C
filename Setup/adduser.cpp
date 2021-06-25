#include "adduser.h"
#include "ui_adduser.h"
#include "../module/uihandler.h"
#include "../module/exglobal.h"


AddUser::AddUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);
    //show_data();

}

void AddUser::show_data()
{
    ui->lEdit_adduser_user->setText(ExGlobal::pUserModel->getCurName());
    ui->lEdit_adduser_disp_name->setText(ExGlobal::pUserModel->getCurDisplayName());
    if (ExGlobal::pUserModel->getCurUserType()==3)
        ui->cBox_adduser_type->setChecked(true);
    else
        ui->cBox_adduser_type->setChecked(false);

}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::on_pushButton_clicked()
{
  //   UIHandler::GoPage(UIHandler::PageId::Page_Setup_User);
    ExGlobal::pUserModel->addUser( ui->lEdit_adduser_user->text(),ui->lEdit_adduser_pwd_2->text(),ui->lEdit_adduser_disp_name->text(),ui->cBox_adduser_type->checkState()?3:1);
     UIHandler::GoPage(UIHandler::PageId::Page_Setup_User);
}

void AddUser::on_pushButton_2_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup_User);
}
