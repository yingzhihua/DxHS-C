#include "network.h"
#include "ui_network.h"
#include "../module/exglobal.h"
#include "../module/uihandler.h"
#include <QMessageBox>

NetWork::NetWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetWork)
{
    ui->setupUi(this);
}

NetWork::~NetWork()
{
    delete ui;
}


void NetWork::on_pushButton_clicked()
{

}

void NetWork::on_pushButton_ok_clicked()
{
    if ((ui->lineEdit_ip1->text()=="")||(ui->lineEdit_ip2->text()=="")||(ui->lineEdit_ip3->text()=="")||(ui->lineEdit_ip4->text()==""))
        {
           QMessageBox::information(this,"提示","IP地址不能有空的项！");
        }
        else if ((ui->lineEdit_mask1->text()=="")||(ui->lineEdit_mask2->text()=="")||(ui->lineEdit_mask3->text()=="")||(ui->lineEdit_mask4->text()==""))
         {
            QMessageBox::information(this,"提示","子网掩码不能有空的项！");
         }
        else if ((ui->lineEdit_gate1->text()=="")||(ui->lineEdit_gate2->text()=="")||(ui->lineEdit_gate3->text()=="")||(ui->lineEdit_gate4->text()==""))
         {
            QMessageBox::information(this,"提示","网关不能有空的项！");
         }
        else
        {
           // busyIndicator.running = true;
            QString  ip = ui->lineEdit_ip1->text()+"."+ui->lineEdit_ip2->text()+"."+ui->lineEdit_ip3->text()+"."+ui->lineEdit_ip4->text();
            QString mask = ui->lineEdit_mask1->text()+"."+ui->lineEdit_mask2->text()+"."+ui->lineEdit_mask3->text()+"."+ui->lineEdit_mask4->text();
            QString gate = ui->lineEdit_gate1->text()+"."+ui->lineEdit_gate2->text()+"."+ui->lineEdit_gate3->text()+"."+ui->lineEdit_gate4->text();
            ExGlobal::getPtr()->setNetIp(ip,mask,gate);
             }
}

void NetWork::on_pushButton_cencel_clicked()
{
   UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
