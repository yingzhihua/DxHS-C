#include "user.h"
#include "ui_user.h"
#include "../module/exglobal.h"
#include "../module/uihandler.h"
#include "updateuser.h"
#include "QMessageBox"
#include "components/twobutton.h"

User::User(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);
    //load_data();


    ui->label->setGeometry(500,150,800,60);
    ui->listWidget_2->setGeometry(500,211,800,400);
    ui->pButton_user_add->setGeometry(1500,350,250,80);
    ui->pButton_user_edit->setGeometry(1500,480,250,80);
    ui->pButton_user_del->setGeometry(1500,610,250,80);
    ui->pButton_user_return->setGeometry(1500,740,250,80);


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

void User::showEvent(QShowEvent *event){
    Q_UNUSED(event);
   load_data();
   connect(TwoButton::getPtr_no_edit(),&TwoButton::queryAck_twobt_no_edit,this,&User::deleteUserBack);
}

void User::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}


void User::on_pButton_user_del_clicked()
{

     TwoButton::display_two_bt_noedit(tr("删除用户"),tr("真的删除用户吗？"),
                         tr("确定"),tr("取消"));
/*
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
*/
   //  ExGlobal::pUserModel->deleteUser(ui->listWidget_2->currentRow());// listView.currentIndex);
   //  load_data();
}

void User::deleteUserBack()
{
    ExGlobal::pUserModel->deleteUser(ui->listWidget_2->currentRow());// listView.currentIndex);
    load_data();
}

void User::on_pButton_user_add_clicked()
{
      ExGlobal::pUserModel->setCurrIndex(ui->listWidget_2->currentRow());
      UIHandler::GoPage(UIHandler::PageId::Page_User_AddUser);
}



void User::on_listWidget_2_currentRowChanged(int currentRow)
{
   // qDebug()<<"currentRowChanged----currentRow=="<<currentRow;

     ExGlobal::pUserModel->setCurrIndex(currentRow);
    // qDebug()<<"currentRowChanged----pUserModel->getCurrIndex=="<<ExGlobal::pUserModel->getCurrIndex();
}

void User::on_pButton_user_edit_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_User_UpdateUser);
}

void User::on_pushButton_canel_clicked()
{

}

void User::on_pushButton_user_return_clicked()
{

}

void User::on_pButton_user_return_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
