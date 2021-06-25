#include "testprocess.h"
#include "ui_testprocess.h"
#include <QPainter>
#include "module/exglobal.h"

TestProcess::TestProcess(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestProcess)
{
    ui->setupUi(this);
    ui->Home_TestProcess_btCancelTest->setGeometry(ExGlobal::contentWidth-299-50,ExGlobal::contentHeight-106-50,299,106);
    ui->Home_TestProcess_btCancelTest->setText(tr("取消测试"));
    ui->Home_TestProcess_lbBK1->setGeometry(358,224,368,457);
    ui->Home_TestProcess_lbBK2->setGeometry(1195,224,368,457);
    ui->Home_TestProcess_lbTestName->setGeometry(368,250,348,130);
    ui->Home_TestProcess_lbTestSerial->setGeometry(368,389,348,130);
    ui->Home_TestProcess_lbTestType->setGeometry(368,534,348,130);
    ui->Home_TestProcess_lbSampleCode->setGeometry(1205,250,348,130);
    ui->Home_TestProcess_lbSampleInfo->setGeometry(1205,389,348,130);
    ui->Home_TestProcess_lbSampleRemark->setGeometry(1205,534,348,130);
    ui->Home_TestProcess_lbTestName->setAlignment(Qt::AlignCenter);
    ui->Home_TestProcess_lbTestSerial->setAlignment(Qt::AlignCenter);
    ui->Home_TestProcess_lbTestType->setAlignment(Qt::AlignCenter);
    ui->Home_TestProcess_lbSampleCode->setAlignment(Qt::AlignCenter);
    ui->Home_TestProcess_lbSampleInfo->setAlignment(Qt::AlignCenter);
    ui->Home_TestProcess_lbSampleRemark->setAlignment(Qt::AlignCenter);
}

TestProcess::~TestProcess()
{
    delete ui;
}

void TestProcess::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter paint(this);
    paint.fillRect(ExGlobal::contentWidth/2-1,240,2,400,QColor(0xd9,0xd9,0xd9));
}

void TestProcess::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    ui->Home_TestProcess_lbTestName->setText(ExGlobal::panelName()+ExGlobal::panelId());
    ui->Home_TestProcess_lbTestSerial->setText(ExGlobal::boxSerial());
    ui->Home_TestProcess_lbTestType->setText(ExGlobal::getCurrSampleName());
    ui->Home_TestProcess_lbSampleCode->setText(ExGlobal::sampleCode());
    ui->Home_TestProcess_lbSampleInfo->setText(ExGlobal::sampleInfo());
    ui->Home_TestProcess_lbSampleRemark->setText(ExGlobal::sampleRemark());

    connect(Sequence::getPtr(),&Sequence::sequenceFinish,this,&TestProcess::on_SequenceFinish);
    Sequence::getPtr()->setStage(Sequence::StageState::Stage_test);
    if (!Sequence::getPtr()->isTesting())
        Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_Test);
}

void TestProcess::hideEvent(QHideEvent *event){
    Q_UNUSED(event);
    Sequence::getPtr()->disconnect(this);
}

void TestProcess::on_Home_TestProcess_btCancelTest_clicked()
{

}

void TestProcess::on_ProcessFinish(int total, int finish)
{
    qDebug()<<"testprocess, on_ProcessFinish"<<total<<finish;

}

void TestProcess::on_SequenceFinish(Sequence::SequenceResult result)
{
    qDebug()<<"testprocess, on_SequenceFinish"<<result;
    if (result == Sequence::SequenceResult::Result_CannelTest_ok){
        UIHandler::GoPage(UIHandler::PageId::Page_Main_Idle);
    }
    else if (result == Sequence::SequenceResult::Result_Test_HandleData){
        UIHandler::UpdateState(UIHandler::StateId::State_Loading_Open);
    }
    else if (result == Sequence::SequenceResult::Result_Test_finish){
        UIHandler::UpdateState(UIHandler::StateId::State_Loading_Close);
        UIHandler::GoPage(UIHandler::PageId::Page_Main_Idle);
    }
}
