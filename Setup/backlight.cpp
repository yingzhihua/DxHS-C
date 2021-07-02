#include "backlight.h"
#include "ui_backlight.h"

#include "../module/exglobal.h"
#include "../module/uihandler.h"

BackLight::BackLight(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BackLight)
{
    ui->setupUi(this);

    ui->label_backlight->setGeometry(300,330,200,60);
    ui->horizontalSlider->setGeometry(550,330,1000,60);
    ui->pushButton->setGeometry(1580,740,300,115);

    ui->horizontalSlider->setValue(ExGlobal::getPtr()->read_backlight());
    ExGlobal::getPtr()->set_backlight(ui->horizontalSlider->value());
}

BackLight::~BackLight()
{
    delete ui;
}

void BackLight::on_horizontalSlider_valueChanged(int value)
{
   // int pwm;
   // pwm=(int)(value*2.55);
   if (value==0)
        value=10;
    ExGlobal::getPtr()->set_backlight(value);
}

void BackLight::on_pushButton_2_clicked()
{

}

void BackLight::on_pushButton_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
