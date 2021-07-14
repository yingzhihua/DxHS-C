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

    ui->label_background->setGeometry(560,255,799,375);

    ui->label->setGeometry(648,359,121,42);
    ui->label_2->setGeometry(604,433,166,42);
    ui->label_3->setGeometry(685,507,84,42);
    ui->lineEdit_ip1->setGeometry(792,351,122,54);
    ui->lineEdit_ip2->setGeometry(923,351,122,54);
    ui->lineEdit_ip3->setGeometry(1054,351,122,54);
    ui->lineEdit_ip4->setGeometry(1185,351,122,54);
    ui->lineEdit_mask1->setGeometry(792,425,122,54);
    ui->lineEdit_mask2->setGeometry(923,425,122,54);
    ui->lineEdit_mask3->setGeometry(1054,425,122,54);
    ui->lineEdit_mask4->setGeometry(1185,425,122,54);

    ui->lineEdit_gate1->setGeometry(792,499,122,54);
    ui->lineEdit_gate2->setGeometry(923,499,122,54);
    ui->lineEdit_gate3->setGeometry(1054,499,122,54);
    ui->lineEdit_gate4->setGeometry(1185,499,122,54);

    ui->lineEdit_ip4->setFocus();

    ui->pushButton_ok->setGeometry(1450,624,299,106);
    ui->pushButton_cencel->setGeometry(1450,754,299,106);


}

NetWork::~NetWork()
{
    delete ui;
}

void NetWork::showEvent(QShowEvent *event){
    Q_UNUSED(event);

    QStringList net = ExGlobal::getPtr()->getNetWork();
    ui->lineEdit_ip1->setText(net[0]);
    ui->lineEdit_ip2->setText(net[1]);
    ui->lineEdit_ip3->setText(net[2]);
    ui->lineEdit_ip4->setText(net[3]);
    ui->lineEdit_gate1->setText(net[4]);
    ui->lineEdit_gate2->setText(net[5]);
    ui->lineEdit_gate3->setText(net[6]);
    ui->lineEdit_gate4->setText(net[7]);
    ui->lineEdit_mask1->setText(net[8]);
    ui->lineEdit_mask2->setText(net[9]);
    ui->lineEdit_mask3->setText(net[10]);
    ui->lineEdit_mask4->setText(net[11]);

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
