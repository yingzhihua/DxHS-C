#include "network.h"
#include "ui_network.h"
#include "../module/exglobal.h"
#include "../module/uihandler.h"
#include <QMessageBox>
#include "components/twobutton.h"

NetWork::NetWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetWork)
{
    ui->setupUi(this);


    ui->label->setGeometry(625,200,200,60);
    ui->label_2->setGeometry(580,300,200,60);
    ui->label_3->setGeometry(670,400,200,60);
    ui->lineEdit_ip1->setGeometry(800,200,100,60);
    ui->lineEdit_ip2->setGeometry(920,200,100,60);
    ui->lineEdit_ip3->setGeometry(1040,200,100,60);
    ui->lineEdit_ip4->setGeometry(1160,200,100,60);
    ui->lineEdit_mask1->setGeometry(800,300,100,60);
    ui->lineEdit_mask2->setGeometry(920,300,100,60);
    ui->lineEdit_mask3->setGeometry(1040,300,100,60);
    ui->lineEdit_mask4->setGeometry(1160,300,100,60);

    ui->lineEdit_gate1->setGeometry(800,400,100,60);
    ui->lineEdit_gate2->setGeometry(920,400,100,60);
    ui->lineEdit_gate3->setGeometry(1040,400,100,60);
    ui->lineEdit_gate4->setGeometry(1160,400,100,60);

    ui->lineEdit_ip4->setFocus();

    ui->pushButton_ok->setGeometry(1180,740,300,115);
    ui->pushButton_cencel->setGeometry(1580,740,300,115);

}

NetWork::~NetWork()
{
    delete ui;
}

void NetWork::showEvent(QShowEvent *event){
    Q_UNUSED(event);

      connect(ExGlobal::getPtr(),&ExGlobal::netFinish,this,&NetWork::ConnectFinish);

}
void NetWork::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}




void NetWork::on_pushButton_clicked()
{

}

void NetWork::on_pushButton_ok_clicked()
{
    if ((ui->lineEdit_ip1->text()=="")||(ui->lineEdit_ip2->text()=="")||(ui->lineEdit_ip3->text()=="")||(ui->lineEdit_ip4->text()==""))
        {
           TwoButton::display_one_bt(tr("提示"),tr("IP地址不能有空的项！"),tr("取消"));
        }
        else if ((ui->lineEdit_mask1->text()=="")||(ui->lineEdit_mask2->text()=="")||(ui->lineEdit_mask3->text()=="")||(ui->lineEdit_mask4->text()==""))
         {
            TwoButton::display_one_bt(tr("提示"),tr("子网掩码不能有空的项！"),tr("取消"));
         }
        else if ((ui->lineEdit_gate1->text()=="")||(ui->lineEdit_gate2->text()=="")||(ui->lineEdit_gate3->text()=="")||(ui->lineEdit_gate4->text()==""))
         {
           TwoButton::display_one_bt(tr("提示"),tr("网关不能有空的项！"),tr("取消"));
         }
        else
        {
           // busyIndicator.running = true;
            QString  ip = ui->lineEdit_ip1->text()+"."+ui->lineEdit_ip2->text()+"."+ui->lineEdit_ip3->text()+"."+ui->lineEdit_ip4->text();
            QString mask = ui->lineEdit_mask1->text()+"."+ui->lineEdit_mask2->text()+"."+ui->lineEdit_mask3->text()+"."+ui->lineEdit_mask4->text();
            QString gate = ui->lineEdit_gate1->text()+"."+ui->lineEdit_gate2->text()+"."+ui->lineEdit_gate3->text()+"."+ui->lineEdit_gate4->text();
             UIHandler::UpdateState(UIHandler::StateId::State_Loading_Open);
            ExGlobal::getPtr()->setNetIp(ip,mask,gate);
          //  UIHandler::UpdateState(UIHandler::StateId::State_Loading_Open);
             }
}

void NetWork::on_pushButton_cencel_clicked()
{
   UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}

void NetWork::ConnectFinish(int val)
{

    UIHandler::UpdateState(UIHandler::StateId::State_Loading_Close);
}
