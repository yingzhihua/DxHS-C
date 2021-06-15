#include "usbfile.h"
#include "ui_usbfile.h"

UsbFile::UsbFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsbFile)
{
    ui->setupUi(this);
}

UsbFile::~UsbFile()
{
    delete ui;
}
