#include "user.h"
#include "ui_user.h"
#include "../module/exglobal.h"
#include "../module/uihandler.h"
#include "updateuser.h"
#include "QMessageBox"


User::User(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);
    load_data();

   // User::pUpdateUser = new UpdateUser();

}

void User::load_data()
{
    ui->listWidget_2->clear();
    for(int ii=0;ii<ExGlobal::pUserModel->rowCount();ii++)
    {
        ExGlobal::pUserModel->setCurrIndex(ii);

        QString user_name,disp_name,item;
        QStringList *user;

        user_name=ExGlobal::pUserModel->getCurName();
        disp_name=ExGlobal::pUserModel->getCurDisplayName();
       //  user_name.resize(25);
        //   dispname.resize(25);
        qDebug()<<"user_name"<<user_name<<user_name.length();
        qDebug()<<"disp_name"<<disp_name<<disp_name.length();

         user_name.resize(30,' ');
         disp_name.resize(18,' ');
         qDebug()<<"user_name"<<user_name<<user_name.length();
         qDebug()<<"disp_name"<<disp_name<<disp_name.length();

        if (ExGlobal::pUserModel->getCurUserType()==3)
            item=user_name+" "+disp_name+"  审核";
        else
            item=user_name+" "+disp_name+"   ";

         qDebug()<<item;
         ui->listWidget_2->addItem(item);
         }
    ui->listWidget_2->setCurrentRow(0);

}

User::~User()
{
    delete ui;
}

void User::on_pButton_user_del_clicked()
{
     QMessageBox msgBox;

     msgBox.setText("删除用户");
     msgBox.setInformativeText("真的删除用户吗？");
     msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
     msgBox.setDefaultButton(QMessageBox::Cancel);
     int ret = msgBox.exec();

     switch (ret) {
        case QMessageBox::Ok:
            ExGlobal::pUserModel->deleteUser(ui->listWidget_2->currentRow());// listView.currentIndex);
            load_data();
            break;
         case QMessageBox::Cancel:

           break;
        default:

            break;
      }

   //  ExGlobal::pUserModel->deleteUser(ui->listWidget_2->currentRow());// listView.currentIndex);
   //  load_data();
}



void User::on_pButton_user_add_clicked()
{
      ExGlobal::pUserModel->setCurrIndex(ui->listWidget_2->currentRow());
      UIHandler::GoPage(UIHandler::PageId::Page_User_AddUser);
}



void User::on_listWidget_2_currentRowChanged(int currentRow)
{
    qDebug()<<"currentRowChanged----currentRow=="<<currentRow;

     ExGlobal::pUserModel->setCurrIndex(currentRow);
     qDebug()<<"currentRowChanged----pUserModel->getCurrIndex=="<<ExGlobal::pUserModel->getCurrIndex();
}

void User::on_pButton_user_edit_clicked()
{

   // ExGlobal::pUserModel->setCurrIndex(ui->listWidget_2->currentRow());
   // qDebug()<<"on_pButton_user_edit_clicked--pUserModel->getCurrIndex=="<<ExGlobal::pUserModel->getCurrIndex();
   //pUpdateUser->show_data();
    // pUpdateUser = new UpdateUser();
    UIHandler::GoPage(UIHandler::PageId::Page_User_UpdateUser);
   // pUpdateUser->show_data();
}
