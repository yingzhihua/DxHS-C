#include "testsetup.h"
#include "ui_testsetup.h"
#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"

TestSetup::TestSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestSetup)
{
    ui->setupUi(this);
   // load_data();
}

TestSetup::~TestSetup()
{
    delete ui;
}

void TestSetup::load_data(void)
{

   ui->cBox_panel->addItems(Sequence::getPtr()->getTestList());
  // ui->cBox_panel->addItem("鼻咽");
  // ui->cBox_panel->addItems(Sequence::getPtr()->getLoopTestList());

   ui->checkBox_qrCode->setChecked(ExGlobal::getPtr()->qrCode());
   ui->spinBox_pox->setValue(ExGlobal::getPtr()->getCaliParam("poxValue"));
}

void TestSetup::on_pButton_machine_back_clicked()
{
      UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}

void TestSetup::on_pButton_testset_set_clicked()
{
    Sequence::getPtr()->sequenceSetPanel(ui->cBox_panel->currentText());
}

void TestSetup::on_pButton_testset_load_clicked()
{
    Sequence::getPtr()->sequenceInit();
}

void TestSetup::on_pButton_testset_test_clicked()
{
    Sequence::getPtr()->lxDebug();
}

void TestSetup::on_checkBox_ChildImage_clicked()
{
    ExGlobal::getPtr()->setChildImage(ui->checkBox_ChildImage->checkState());
}

void TestSetup::on_checkBox_AutoFocus_clicked()
{
    ExGlobal::getPtr()->setAutoFocus(ui->checkBox_AutoFocus->checkState());
}

void TestSetup::on_checkBox_qrCode_clicked()
{
    ExGlobal::getPtr()->setQrCode(ui->checkBox_qrCode->checkState());
}

void TestSetup::on_spinBox_pox_valueChanged(int arg1)
{
    ExGlobal::getPtr()->updateCaliParam("poxValue",arg1);
}

void TestSetup::on_pButton_testset_2_Q_clicked()
{

    ui->label_messageTest->setText("");
      Sequence::getPtr()->qrSet(ui->checkBox_light->checkState(),ui->checkBox_cut_image->checkState(),ui->checkBox_handle_image->checkState(),ui->spinBox_binF->value(),ui->spinBox_pox->value());
      Sequence::getPtr()->sequenceDo(Sequence::getPtr()->Sequence_QrDecode);

}
