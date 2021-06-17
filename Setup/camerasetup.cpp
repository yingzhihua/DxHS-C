#include "camerasetup.h"
#include "ui_camerasetup.h"

CameraSetup::CameraSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSetup)
{
    ui->setupUi(this);
}

CameraSetup::~CameraSetup()
{
    delete ui;
}
