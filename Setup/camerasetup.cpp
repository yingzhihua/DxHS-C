#include "camerasetup.h"
#include "ui_camerasetup.h"
#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"



CameraSetup::CameraSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSetup)
{
    ui->setupUi(this);
    ui->Setup_CameraSetup_lbCam->setGeometry(20,20,931,761);
    ui->Setup_CameraSetup_lbCam->setAlignment(Qt::AlignCenter);
}

CameraSetup::~CameraSetup()
{
    delete ui;
}

void CameraSetup::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    ui->sBox_time->setValue(Sequence::getPtr()->getAbs());  //曝光时间
    ui->sBox_gain->setValue(Sequence::getPtr()->getGain());  //增益
    ui->sBox_focus->setValue(ExGlobal::getPtr()->getCaliParam("CamFocus")); //焦距
    if (Sequence::getPtr()->getWhiteBalance()==0)   //是否打开自动白平衡
       ui->cBox_open_auto->setChecked(true);
    else
       ui->cBox_open_auto->setChecked(false);
    ui->sBox_temp->setValue(Sequence::getPtr()->getWhiteBalance()); //白平衡温度


    Sequence::getPtr()->changeTitle("摄像头测试");

    ui->lineEdit_x->setText(QString::number(ExGlobal::getPtr()->getCaliParam("FocusX")));
    ui->lineEdit_y->setText(QString::number(ExGlobal::getPtr()->getCaliParam("FocusY")));
    ui->lineEdit_w->setText(QString::number(ExGlobal::getPtr()->getCaliParam("FocusWidth")));
    ui->lineEdit_h->setText(QString::number(ExGlobal::getPtr()->getCaliParam("FocusHeight")));

    connect(Sequence::getPtr(),&Sequence::callRefeshView,this,&CameraSetup::CameraView);
    ui->Setup_CameraSetup_lbCam->setText(tr("预览停止"));
}
void CameraSetup::hideEvent(QHideEvent *event){
    Q_UNUSED(event);
    Sequence::getPtr()->stopView();
    Sequence::getPtr()->actionDo("Light",1,0,0,0);
    bView = false;
    Sequence::getPtr()->disconnect(this);
}

void CameraSetup::on_pButton_backl_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}

void CameraSetup::on_pButton_open_view_clicked()
{
   // viewText.text = qsTr("图像加载中。。。");    
    ui->Setup_CameraSetup_lbCam->setText(tr("图像加载中。。。"));
    Sequence::getPtr()->startView(ui->cBox_show_box->checkState()?1:0);
    bView = true;
}

void CameraSetup::on_pButton_stop_view_clicked()
{    
    Sequence::getPtr()->stopView();
    bView = false;
    //viewText.text = "";
    //cameraBox.source = "";
    ui->Setup_CameraSetup_lbCam->setText(tr("预览停止"));
}

void CameraSetup::on_pButton_open_light_clicked()
{
    Sequence::getPtr()->actionDo("Light",5,0,0,0);
}

void CameraSetup::on_pButton_close_light_clicked()
{
    Sequence::getPtr()->actionDo("Light",1,0,0,0);
}

void CameraSetup::on_pButton_auto_focus_clicked()
{
    bView = true;
    Sequence::getPtr()->autoFocus();
}

void CameraSetup::on_pButton_time_clicked()
{
   Sequence::getPtr()->setAbs(ui->sBox_time->value());
}

void CameraSetup::on_pButton_gain_clicked()
{
    Sequence::getPtr()->setGain(ui->sBox_gain->value());
}

void CameraSetup::on_pButton_focus_clicked()
{
    if (Sequence::getPtr()->actionDo("Focus",2,ui->sBox_focus->value()+2600,0,0))
        ExGlobal::getPtr()->updateCaliParam("CamFocus",ui->sBox_focus->value());
}

void CameraSetup::on_pButton_reset_clicked()
{
    if (Sequence::getPtr()->actionDo("Focus",1,0,0,0))
    {
        ui->sBox_focus->setValue(0);
        ExGlobal::getPtr()->updateCaliParam("CamFocus",ui->sBox_focus->value());
    }
}

void CameraSetup::on_pButton_temp_clicked()
{
    Sequence::getPtr()->setWhiteBalance( ui->cBox_open_auto->checkState()?0:ui->sBox_temp->value());
}

void CameraSetup::on_pButton_area_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("FocusX",ui->lineEdit_x->text().toInt());
    ExGlobal::getPtr()->updateCaliParam("FocusY",ui->lineEdit_y->text().toInt());
    ExGlobal::getPtr()->updateCaliParam("FocusWidth",ui->lineEdit_w->text().toInt());
    ExGlobal::getPtr()->updateCaliParam("FocusHeight",ui->lineEdit_h->text().toInt());
    Sequence::getPtr()->setFocus();
}

void CameraSetup::on_cBox_show_box_clicked()
{
    if (ui->cBox_show_box->checkState())
        Sequence::getPtr()->setViewType(1);
    else
        Sequence::getPtr()->setViewType(0);
}

void CameraSetup::CameraView(QImage img)
{
    if (bView)
        ui->Setup_CameraSetup_lbCam->setPixmap(QPixmap::fromImage(img.scaled(ui->Setup_CameraSetup_lbCam->size(),Qt::KeepAspectRatio)));
}
