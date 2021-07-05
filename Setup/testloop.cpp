#include "testloop.h"
#include "ui_testloop.h"
#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"

TestLoop::TestLoop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestLoop)
{
    ui->setupUi(this);
}

TestLoop::~TestLoop()
{
    delete ui;
}

void TestLoop::load_data(void)
{

    //Sequence::getPtr()->LoopTestList;
 // ui->comboBox_panel
   ui->comboBox_panel->addItems(Sequence::getPtr()->getLoopTestList());
    ui->pButton_machine_back->setGeometry(1580,740,300,115);
}

void TestLoop::on_pButton_machine_back_clicked()
{
     UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}

void TestLoop::on_pButton_enter_test_clicked()
{
    ui->comboBox_panel->setEnabled(false);
    ui->spinBox_test_count->setEnabled(false);
    Sequence::getPtr()->loopTest(ui->comboBox_panel->currentText(),ui->spinBox_test_count->value());

}

void TestLoop::on_pButton_exit_test_clicked()
{
     Sequence::getPtr()->sequenceCancel();
}
