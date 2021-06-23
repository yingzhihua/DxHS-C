#include "adminpassword.h"
#include "ui_adminpassword.h"
#include "../module/uihandler.h"
#include <QMessageBox>
AdminPassword::AdminPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminPassword)
{
    ui->setupUi(this);
    userModel= new UserModel();

}

AdminPassword::~AdminPassword()
{
    delete ui;
}

void AdminPassword::on_pushButton_OK_clicked()
{
    if (ui->lineEdit_oldpwd->text()=="")
    {
        QMessageBox msgBox;
      //  msgBox.resize(1000,500);
        msgBox.size().setWidth(500);
        msgBox.size().setHeight(300);
        //msgBox.information(this,"title","message");
        msgBox.setText("Messageee");
        msgBox.exec();
    }
       // MessageBox(NULL,"密码不能是空","",1);
    //userModel->updatePassword(ui->lineEdit_oldpwd,ui->lineEdit_newpwd);
    userModel->updatePassword(ui->lineEdit_oldpwd->text(),ui->lineEdit_newpwd->text());
}

void AdminPassword::on_pushButton_Cencel_clicked()
{
      UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
