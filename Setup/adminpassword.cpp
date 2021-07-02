#include "adminpassword.h"
#include "ui_adminpassword.h"
#include "../module/uihandler.h"
#include "../module/exglobal.h"
#include <QMessageBox>
AdminPassword::AdminPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminPassword)
{
    ui->setupUi(this);
    userModel= new UserModel();


    int init_x = (ExGlobal::contentWidth-810)/2;
    int init_y = (ExGlobal::contentHeight-565)/2+150;
    setGeometry(init_x,init_y,810,565);
    ui->label_oldpwd->setGeometry(490,200,200,60);
    ui->lineEdit_oldpwd->setGeometry(700,200,500,60);
    ui->label_newpwd->setGeometry(490,300,200,60);
    ui->lineEdit_newpwd->setGeometry(700,300,500,60);
    ui->label_newpwd_2->setGeometry(450,400,230,60);
    ui->lineEdit_newpwd_2->setGeometry(700,400,500,60);
    ui->pushButton_OK->setGeometry(1180,740,300,115);
    ui->pushButton_Cencel->setGeometry(1580,740,300,115);

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
