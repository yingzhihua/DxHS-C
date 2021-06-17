#include "light.h"
#include "ui_light.h"

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
