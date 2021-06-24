#include "boxready.h"
#include "ui_boxready.h"
#include <QListView>
#include "components/threequery.h"

BoxReady::BoxReady(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoxReady)
{
    ui->setupUi(this);
    ui->Home_BoxReady_btStart->setGeometry(850,287,219,224);
    ui->Home_BoxReady_lbTips->setGeometry(ui->Home_BoxReady_btStart->x()+(219-350)/2,ui->Home_BoxReady_btStart->y()+224+28,350,50);
    ui->Home_BoxReady_btPen->setGeometry(1621,606,75,70);
    ui->Home_BoxReady_btCancelTest->setGeometry(ExGlobal::contentWidth-299-50,ExGlobal::contentHeight-106-50,299,106);
    ui->Home_BoxReady_btCancelTest->setText(tr("取消测试"));
    ui->Home_BoxReady_lbBK1->setGeometry(358,224,368,457);
    ui->Home_BoxReady_lbBK2->setGeometry(1195,224,368,457);
    ui->Home_BoxReady_lbTestName->setGeometry(368,250,348,130);
    ui->Home_BoxReady_lbTestSerial->setGeometry(368,389,348,130);
    ui->Home_BoxReady_cbTestType->setGeometry(390,570,300,60);
    ui->Home_BoxReady_cbTestType->setView(new QListView);
    ui->Home_BoxReady_lbSampleCode->setGeometry(1205,250,348,130);
    ui->Home_BoxReady_lbSampleInfo->setGeometry(1205,389,348,130);
    ui->Home_BoxReady_lbSampleRemark->setGeometry(1205,534,348,130);
    ui->Home_BoxReady_lbTips->setAlignment(Qt::AlignCenter);
    ui->Home_BoxReady_lbTips->setWordWrap(true);
    ui->Home_BoxReady_lbTestName->setAlignment(Qt::AlignCenter);
    ui->Home_BoxReady_lbTestSerial->setAlignment(Qt::AlignCenter);
    ui->Home_BoxReady_lbSampleCode->setAlignment(Qt::AlignCenter);
    ui->Home_BoxReady_lbSampleInfo->setAlignment(Qt::AlignCenter);
    ui->Home_BoxReady_lbSampleRemark->setAlignment(Qt::AlignCenter);

}

BoxReady::~BoxReady()
{
    delete ui;
}

void BoxReady::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    ui->Home_BoxReady_lbTestName->setText(ExGlobal::panelName()+ExGlobal::panelId());
    ui->Home_BoxReady_lbTestSerial->setText(ExGlobal::boxSerial());
    ui->Home_BoxReady_cbTestType->clear();
    ui->Home_BoxReady_cbTestType->addItems(ExGlobal::getSampleTypeArr());

    ui->Home_BoxReady_lbSampleCode->setText(ExGlobal::sampleCode());
    ui->Home_BoxReady_lbSampleInfo->setText(ExGlobal::sampleInfo());
    ui->Home_BoxReady_lbSampleRemark->setText(ExGlobal::sampleRemark());
    Sequence::getPtr()->setStage(Sequence::StageState::Stage_ready);
    Sequence::getPtr()->footerNotify(true,true,true);
    ChangedTips();
    connect(ThreeQuery::getPtr(),&ThreeQuery::queryAck,this,&BoxReady::on_Sample_Changed);
}

void BoxReady::ChangedTips()
{
    if (ExGlobal::sampleCode().isEmpty()){
        ui->Home_BoxReady_lbTips->setText(tr("样本号不能为空 请输入样本号"));
        ui->Home_BoxReady_lbTips->setStyleSheet("color: #FC7052");
        ui->Home_BoxReady_btStart->setEnabled(false);
    }
    else{
        ui->Home_BoxReady_lbTips->setText(tr("开始测试"));
        ui->Home_BoxReady_lbTips->setStyleSheet("color: #4c4c4c");
        ui->Home_BoxReady_btStart->setEnabled(true);
    }
}

void BoxReady::on_Home_BoxReady_btPen_clicked()
{
    ThreeQuery::display(tr("样本信息录入"),tr("样本号"),tr("样本信息"),tr("样本备注"));
}

void BoxReady::on_Sample_Changed(QString res1, QString res2, QString res3)
{
    qDebug()<<"on_Sample_Changed"<<res1;
    ExGlobal::getPtr()->setSampleCode(res1);
    ExGlobal::getPtr()->setSampleInfo(res2);
    ExGlobal::getPtr()->setSampleRemark(res3);

    ui->Home_BoxReady_lbSampleCode->setText(res1);
    ui->Home_BoxReady_lbSampleInfo->setText(res2);
    ui->Home_BoxReady_lbSampleRemark->setText(res3);
    ChangedTips();
}

void BoxReady::on_Home_BoxReady_btStart_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Main_Test);
}

void BoxReady::on_Home_BoxReady_btCancelTest_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Main_Idle);
}
