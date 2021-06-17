#include "backlight.h"
#include "ui_backlight.h"
#include "../module/exglobal.h"

BackLight::BackLight(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BackLight)
{
    ui->setupUi(this);
}

BackLight::~BackLight()
{
    delete ui;
}

void BackLight::on_horizontalSlider_valueChanged(int value)
{
    int pwm;
    pwm=(int)(value*2.55);
    if (pwm==0)
        pwm=5;
    ExGlobal::set_backlight(pwm);
}
