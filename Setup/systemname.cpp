#include "systemname.h"
#include "ui_systemname.h"
#include "../module/exglobal.h"
#include "../module/uihandler.h"

SystemName::SystemName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemName)
{
    // QFont font_edit=ui->pushButton_2->font();
    ui->setupUi(this);
    ui->pushButton->text()="确定";


    int init_x = (ExGlobal::contentWidth-810)/2;
    int init_y = (ExGlobal::contentHeight-565)/2+150;
    setGeometry(init_x,init_y,810,565);

    ui->label->setGeometry(561,255,798,375);

    ui->label_hospital->setGeometry(675,346,165,37);
    ui->lineEdit_hospital->setGeometry(874,328,404,64);
    ui->label_device->setGeometry(675,435,168,37);
    ui->lineEdit_device->setGeometry(874,420,404,64);
    ui->label_print->setGeometry(631,520,207,37);
    ui->comboBox_print->setGeometry(874,507,404,64);

   // ui->lineEdit_device->setFont(font_edit);
   // ui->lineEdit_device->setFont(font_edit);
   // ui->pushButton->setGeometry(1180,740,300,115);
   // ui->pushButton_2->setGeometry(1580,740,300,115);
    /*
    ui->label_hospital->setGeometry(450,200,200,60);
    ui->lineEdit_hospital->setGeometry(700,200,500,60);
    ui->label_device->setGeometry(450,300,200,60);
    ui->lineEdit_device->setGeometry(700,300,500,60);
    ui->label_print->setGeometry(420,400,220,60);
    ui->comboBox_print->setGeometry(700,400,500,60);
    ui->pushButton->setGeometry(1180,740,300,115);
    ui->pushButton_2->setGeometry(1580,740,300,115);
*/
   // ui->label->setVisible(false);
   // ui->pushButton->setGeometry(1050,704,299,106);
   // ui->pushButton_2->setGeometry(1450,704,299,106);
  //  ui->pushButton->setGeometry(1050,754,299,106);
    ui->pushButton->setGeometry(1450,624,299,106);
    ui->pushButton_2->setGeometry(1450,754,299,106);
}

SystemName::~SystemName()
{
    delete ui;
}

void SystemName::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    ui->lineEdit_hospital->setText(ExGlobal::getPtr()->hospitalName());
    ui->lineEdit_device->setText(ExGlobal::getPtr()->sysName());
    ui->comboBox_print->setCurrentIndex(ExGlobal::getPtr()->printType());
}

void SystemName::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}

void SystemName::on_pushButton_clicked()
{
    ExGlobal::getPtr()->setHospitalName(ui->lineEdit_hospital->text());
    ExGlobal::getPtr()->setSysName(ui->lineEdit_device->text());
    ExGlobal::getPtr()->setPrintType(ui->comboBox_print->currentIndex());
    //ui->lineEdit_hospital->setText(ExGlobal::getPtr()->hospitalName());
}

void SystemName::on_pushButton_2_clicked()
{
     //delete ui;
     UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
