#include "user.h"
#include "ui_user.h"
#include "../module/exglobal.h"
#include "../module/uihandler.h"
#include "updateuser.h"
#include "QMessageBox"
#include "components/twobutton.h"
#include <QStandardItemModel>


User::User(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User)
{
    ui->setupUi(this);
    //load_data();

    ui->label_background->setGeometry(566,157,788,581);
    ui->label_name->setGeometry(649,187,85,42);
    ui->label_dis_name->setGeometry(909,187,125,42);
    ui->label_right->setGeometry(1200,187,85,42);
    ui->listWidget_2->setGeometry(566,253,789,346);

    ui->pButton_user_add->setGeometry(1114,631,220,72);
    ui->pButton_user_edit->setGeometry(859,631,220,72);
    ui->pButton_user_del->setGeometry(607,631,220,72);
    ui->pButton_user_return->setGeometry(1500,740,250,80);
    ui->pButton_user_return->setVisible(false);
    ui->pushButton_back->setGeometry(1450,754,299,106);

}

/*
void User::mode_data()
{
    QStandardItem  *item[10];

     QList<QStandardItem *> itemList;

    item[0]=new QStandardItem();
    item[1]=new QStandardItem();
    item[2]=new QStandardItem();
    QStandardItemModel *model = new QStandardItemModel;
  //  model->setColumnCount(3);
   // QListView *view=new QListView;
  // model->appendRow()

    item[0]->setText("中国");
    item[1]->setText("广东");
    item[2]->setText("深圳");
    itemList.append(item[0]);
    itemList.append(item[1]);
     itemList.append(item[2]);

   model->appendRow(itemList);
   // model->insertColumn(0,itemList);
    // ui->listView->setModelColumn(3);
    ui->listView->setModel(model);


}
*/
void User::load_data()
{
     ui->listWidget_2->setSpacing(5);
     ui->listWidget_2->setFixedSize(789,346);


    QStringList  strlist1={"中国","广东","深圳市"};
    ui->listWidget_2->clear();
    ui->listWidget_2->setStyleSheet("QScrollBar::vertical{width:40px}");
     qDebug()<<"currIndex0=============="<<currIndex;
    for(int ii=0;ii<ExGlobal::pUserModel->rowCount();ii++)
    {
        ExGlobal::pUserModel->setCurrIndex(ii);

        QString user_name,disp_name,item;
        QStringList *user;

        user_name=ExGlobal::pUserModel->getCurName();
        disp_name=ExGlobal::pUserModel->getCurDisplayName();
       //  user_name.resize(25);
        //   dispname.resize(25);
        user_name="     "+user_name;
        //qDebug()<<"user_name"<<user_name<<user_name.length();
       // qDebug()<<"disp_name"<<disp_name<<disp_name.length();

         user_name.resize(30,' ');
         disp_name.resize(18,' ');
         //qDebug()<<"user_name"<<user_name<<user_name.length();
        // qDebug()<<"disp_name"<<disp_name<<disp_name.length();

        if (ExGlobal::pUserModel->getCurUserType()==3)
            item=user_name+" "+disp_name+" 审核";
        else
            item=user_name+" "+disp_name+" ";

         qDebug()<<item;
         ui->listWidget_2->addItem(item);
        // ui->listView->setmo
         }
   // ui->listWidget_2->setCurrentRow(ExGlobal::pUserModel->getCurrIndex());
    if (currIndex==-1)currIndex=ExGlobal::pUserModel->getUserNum()-1;
    ui->listWidget_2->setCurrentRow(currIndex);
    ExGlobal::pUserModel->setCurrIndex(currIndex);
    qDebug()<<"currIndex1=============="<<currIndex;
  //  ui->listWidget_2->addItems(strlist1);

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
    currIndex=ui->listWidget_2->currentRow();
     if (ExGlobal::pUserModel->getUserNum()>0)
        TwoButton::display_two_bt_noedit(tr("删除用户"),tr("真的删除用户吗？"),
                         tr("确定"),tr("取消"));



}

void User::deleteUserBack()
{
    if (ExGlobal::pUserModel->getUserNum()>0)
    {
      ExGlobal::pUserModel->deleteUser(currIndex);//ui->listWidget_2->currentRow());// listView.currentIndex);
     }
    load_data();
   // ui->listWidget_2->setCurrentRow(0);
   // ExGlobal::pUserModel->setCurrIndex(0);
}

void User::on_pButton_user_add_clicked()
{
     // if (ExGlobal::pUserModel->getUserNum()>0)
       //  ExGlobal::pUserModel->setCurrIndex(ui->listWidget_2->currentRow());
   currIndex=ExGlobal::pUserModel->getCurrIndex();
      UIHandler::GoPage(UIHandler::PageId::Page_User_AddUser);
}



void User::on_listWidget_2_currentRowChanged(int currentRow)
{
   // qDebug()<<"currentRowChanged----currentRow=="<<currentRow;
      ExGlobal::pUserModel->setCurrIndex(currentRow);
      currIndex=currentRow;
    // qDebug()<<"currentRowChanged----pUserModel->getCurrIndex=="<<ExGlobal::pUserModel->getCurrIndex();
}

void User::on_pButton_user_edit_clicked()
{
   if (ExGlobal::pUserModel->getUserNum()>0)
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

void User::on_pushButton_back_clicked()
{
   UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
