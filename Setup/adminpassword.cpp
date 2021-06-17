#include "adminpassword.h"
#include "ui_adminpassword.h"
#include "../module/uihandler.h"

AdminPassword::AdminPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminPassword)
{
    ui->setupUi(this);
}

AdminPassword::~AdminPassword()
{
    delete ui;
}

void AdminPassword::on_pushButton_OK_clicked()
{
   // userModel.updatePassword(oldpassword.text,password.text)
}

void AdminPassword::on_pushButton_Cencel_clicked()
{
      UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
