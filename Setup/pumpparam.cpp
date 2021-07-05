#include "pumpparam.h"
#include "ui_pumpparam.h"
#include "../module/exglobal.h"
#include "../module/uihandler.h"
#include "../module/sequence.h"
#include <QMessageBox>

#include "components/twobutton.h"

PumpParam::PumpParam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PumpParam)
{
    ui->setupUi(this);
    load_data();

    ui->pushButton->setGeometry(1180,740,300,115);
    ui->pushButton_2->setGeometry(1580,740,300,115);
}

PumpParam::~PumpParam()
{
    delete ui;
}

void PumpParam::load_data()
{
    ui->label_pump1_data->setNum(ExGlobal::getCaliParam("VPump1"));
    ui->lEdit_pump1->setText(QString::number(ExGlobal::getCaliParam("VPump1"),10));
    ui->label_pump2_data->setNum(ExGlobal::getCaliParam("VPump2"));
    ui->lEdit_pump2->setText(QString::number(ExGlobal::getCaliParam("VPump2"),10));
    ui->label_pump3_data->setNum(ExGlobal::getCaliParam("VPump3"));
    ui->lEdit_pump3->setText(QString::number(ExGlobal::getCaliParam("VPump3"),10));
    ui->label_pump4_data->setNum(ExGlobal::getCaliParam("VPump4"));
    ui->lEdit_pump4->setText(QString::number(ExGlobal::getCaliParam("VPump4"),10));
    ui->label_pump5_data->setNum(ExGlobal::getCaliParam("VPump5"));
    ui->lEdit_pump5->setText(QString::number(ExGlobal::getCaliParam("VPump5"),10));

    ui->label_pump6_data->setNum(ExGlobal::getCaliParam("VPump6"));
    ui->lEdit_pump6->setText(QString::number(ExGlobal::getCaliParam("VPump6"),10));
    ui->label_pump7_data->setNum(ExGlobal::getCaliParam("VPump7"));
    ui->lEdit_pump7->setText(QString::number(ExGlobal::getCaliParam("VPump7"),10));
    ui->label_pump8_data->setNum(ExGlobal::getCaliParam("VPump8"));
    ui->lEdit_pump8->setText(QString::number(ExGlobal::getCaliParam("VPump8"),10));
    ui->label_pump9_data->setNum(ExGlobal::getCaliParam("VPump9"));
    ui->lEdit_pump9->setText(QString::number(ExGlobal::getCaliParam("VPump9"),10));
    ui->label_pump10_data->setNum(ExGlobal::getCaliParam("VPump10"));
    ui->lEdit_pump10->setText(QString::number(ExGlobal::getCaliParam("VPump10"),10));


    ui->label_pump11_data->setNum(ExGlobal::getCaliParam("VPump11"));
    ui->lEdit_pump11->setText(QString::number(ExGlobal::getCaliParam("VPump11"),10));
    ui->label_pump12_data->setNum(ExGlobal::getCaliParam("VPump12"));
    ui->lEdit_pump12->setText(QString::number(ExGlobal::getCaliParam("VPump12"),10));
    ui->label_pump13_data->setNum(ExGlobal::getCaliParam("VPump13"));
    ui->lEdit_pump13->setText(QString::number(ExGlobal::getCaliParam("VPump13"),10));
    ui->label_pump14_data->setNum(ExGlobal::getCaliParam("VPump14"));
    ui->lEdit_pump14->setText(QString::number(ExGlobal::getCaliParam("VPump14"),10));
    ui->label_pump15_data->setNum(ExGlobal::getCaliParam("VPump15"));
    ui->lEdit_pump15->setText(QString::number(ExGlobal::getCaliParam("VPump15"),10));

}

void PumpParam::on_pushButton_clicked()
{
    ExGlobal::updateCaliParam("VPump1",(ui->lEdit_pump1->text()).toInt());
    ExGlobal::updateCaliParam("VPump2",(ui->lEdit_pump2->text()).toInt());
    ExGlobal::updateCaliParam("VPump3",(ui->lEdit_pump3->text()).toInt());
    ExGlobal::updateCaliParam("VPump4",(ui->lEdit_pump4->text()).toInt());
    ExGlobal::updateCaliParam("VPump5",(ui->lEdit_pump5->text()).toInt());
    ExGlobal::updateCaliParam("VPump6",(ui->lEdit_pump6->text()).toInt());
    ExGlobal::updateCaliParam("VPump7",(ui->lEdit_pump7->text()).toInt());
    ExGlobal::updateCaliParam("VPump8",(ui->lEdit_pump8->text()).toInt());
    ExGlobal::updateCaliParam("VPump9",(ui->lEdit_pump9->text()).toInt());
    ExGlobal::updateCaliParam("VPump10",(ui->lEdit_pump10->text()).toInt());
    ExGlobal::updateCaliParam("VPump11",(ui->lEdit_pump11->text()).toInt());
    ExGlobal::updateCaliParam("VPump12",(ui->lEdit_pump12->text()).toInt());
    ExGlobal::updateCaliParam("VPump13",(ui->lEdit_pump13->text()).toInt());
    ExGlobal::updateCaliParam("VPump14",(ui->lEdit_pump14->text()).toInt());
    ExGlobal::updateCaliParam("VPump15",(ui->lEdit_pump15->text()).toInt());
/*
    vpumpr1Text.text = ExGlobal.getCaliParam("VPump1");
    vpumpr2Text.text = ExGlobal.getCaliParam("VPump2");
    vpumpr3Text.text = ExGlobal.getCaliParam("VPump3");
    vpumpr4Text.text = ExGlobal.getCaliParam("VPump4");
    vpumpr5Text.text = ExGlobal.getCaliParam("VPump5");
    vpumpr6Text.text = ExGlobal.getCaliParam("VPump6");
    vpumpr7Text.text = ExGlobal.getCaliParam("VPump7");
    vpumpr8Text.text = ExGlobal.getCaliParam("VPump8");
    vpumpr9Text.text = ExGlobal.getCaliParam("VPump9");
    vpumpr10Text.text = ExGlobal.getCaliParam("VPump10");
    vpumpr11Text.text = ExGlobal.getCaliParam("VPump11");
    vpumpr12Text.text = ExGlobal.getCaliParam("VPump12");
    vpumpr13Text.text = ExGlobal.getCaliParam("VPump13");
    vpumpr14Text.text = ExGlobal.getCaliParam("VPump14");
    vpumpr15Text.text = ExGlobal.getCaliParam("VPump15");
*/
   // sequence->checkParam(true);
    Sequence::getPtr()->checkParam(true);

    TwoButton::display_one_bt(tr("流体参数"),tr("数据保存完成！"),tr("返回"));
}

void PumpParam::on_pushButton_2_clicked()
{
     UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
