#include "light.h"
#include "ui_light.h"
#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"


Light::Light(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Light)
{
    ui->setupUi(this);
}

Light::~Light()
{
    delete ui;
}

void Light::load_data(void)
{
    ui->cBox_panel->addItems(Sequence::getPtr()->onePointTestList());

}

void Light::on_pButton_backl_clicked()
{
     UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}

void Light::on_pButton_work_clicked()
{
   // doingBox.show(qsTr("舱门移动到工作位"))
    Sequence::getPtr()->sequenceDo(Sequence::getPtr()->Sequence_DoorToWork);
}

void Light::on_pButton_hard_clicked()
{
   // doingBox.show(qsTr("舱门回到硬件零点"))
    Sequence::getPtr()->sequenceDo(Sequence::getPtr()->Sequence_DoorFromWork);
}

void Light::on_pButton_set_clicked()
{
   // console.log("set onePointPanelIndex",panelcombo.currentIndex)
    Sequence::getPtr()->setOnePointPanelIndex(ui->cBox_panel->currentIndex());
}

void Light::on_pButton_set_xy_clicked()
{

    ExGlobal::getPtr()->updateCaliParam("LightCX",ui->lEdit_X->text().toInt());
    ExGlobal::getPtr()->updateCaliParam("LightCY",ui->lEdit_Y->text().toInt());
    ExGlobal::getPtr()->updateCaliParam("LightR",ui->lEdit_rad->text().toInt());
    Sequence::getPtr()->setLight();

}

void Light::on_pButton_get_light_clicked()
{
  //  doingBox.show(qsTr("获取荧光亮度"))
    Sequence::getPtr()->startLight();
}
