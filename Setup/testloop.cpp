#include "testloop.h"
#include "ui_testloop.h"
#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"
#include "components/twobutton.h"

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

void TestLoop::sequenceFinish(Sequence::SequenceResult result)
{
    if(result == Sequence::SequenceResult::Result_LoopTest_finish){
        TwoButton::display_one_bt(tr("测试"),tr("耐久性测试完成！"),tr("返回"));
        ui->comboBox_panel->setEnabled(true);
        ui->spinBox_test_count->setEnabled(true);
    }
    else if(result == Sequence::SequenceResult::Result_CannelTest_ok)
    {
        TwoButton::display_one_bt(tr("测试"),tr("耐久性测试停止！"),tr("返回"));
        ui->comboBox_panel->setEnabled(true);
        ui->spinBox_test_count->setEnabled(true);
    }
}

void TestLoop::load_data(void)
{
    ui->comboBox_panel->addItems(Sequence::getPtr()->getLoopTestList());
    ui->pButton_machine_back->setGeometry(1580,740,300,115);

    // connect(Sequence::getPtr(),&Sequence::doorStateChanged,this,&TestLoop::sequenceFinish);
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
