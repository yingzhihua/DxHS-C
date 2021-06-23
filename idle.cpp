#include "idle.h"
#include "ui_idle.h"

Idle::Idle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Idle)
{
    ui->setupUi(this);
    ui->Home_Idle_btOpenDoor->setGeometry((ExGlobal::contentWidth-219)/2,(ExGlobal::contentHeight-224)/2,219,224);
    ui->Home_Idle_lbBoxState->setGeometry((ExGlobal::contentWidth-400)/2,ui->Home_Idle_btOpenDoor->y()+280,400,60);
    ui->Home_Idle_lbBoxState->setAlignment(Qt::AlignCenter);
    ui->Home_Idle_lbBoxState->setText(tr("未检测到试剂盒"));
    ui->Home_Idle_btQuit->setGeometry(ExGlobal::contentWidth-299-50,ExGlobal::contentHeight-106-50,299,106);
    ui->Home_Idle_btQuit->setText(tr("关机"));
    ui->Home_Idle_btLogout->setGeometry(ui->Home_Idle_btQuit->x(),ui->Home_Idle_btQuit->y()-106-30,299,106);
    ui->Home_Idle_btLogout->setText(tr("退出登录"));
}

Idle::~Idle()
{
    delete ui;
}
static int timerid = 0;
void Idle::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    connect(Sequence::getPtr(),&Sequence::boxStateChanged,this,&Idle::StateUpdate);
    connect(Sequence::getPtr(),&Sequence::doorStateChanged,this,&Idle::StateUpdate);
    connect(Sequence::getPtr(),&Sequence::sequenceFinish,this,&Idle::sequenceFinish);
    StateUpdate();
    Sequence::getPtr()->actionDo("Query",3,0,0,0);
    Sequence::getPtr()->footerNotify(true,true,true);
}

void Idle::hideEvent(QHideEvent *event){
    Q_UNUSED(event);
    Sequence::getPtr()->disconnect(this);
    if (timerid != 0){
        killTimer(timerid);
        timerid = 0;
    }
}

void Idle::on_Home_Idle_btOpenDoor_clicked()
{
    if (Sequence::getPtr()->readDoorState())
        Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_CloseBox);
    else
        Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_OpenBox);
    UIHandler::UpdateState(UIHandler::StateId::State_Loading_Open);
}

void Idle::on_Home_Idle_btLogout_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Main_Login);
}

void Idle::on_Home_Idle_btQuit_clicked()
{
    ExGlobal::exClose();
}

void Idle::StateUpdate()
{
    qDebug()<<"idle stateupdate"<<Sequence::getPtr()->readBoxState()<<Sequence::getPtr()->readDoorState();
    if (Sequence::getPtr()->readBoxState())
        ui->Home_Idle_lbBoxState->setText(tr("试剂盒就绪"));
    else
        ui->Home_Idle_lbBoxState->setText(tr("未检测到试剂盒"));
    if (Sequence::getPtr()->readDoorState())
        timerid = startTimer(1000);
    else if(timerid != 0){
        killTimer(timerid);
        timerid = 0;
    }
}

void Idle::sequenceFinish(Sequence::SequenceResult result)
{
    qDebug()<<"idle,sequenceFinish,result="<<result;
    if (result == Sequence::SequenceResult::Result_OpenBox_ok)
    {
        UIHandler::UpdateState(UIHandler::StateId::State_Loading_Close);
    }
    else if(result == Sequence::SequenceResult::Result_CloseBox_ok)
    {
        UIHandler::UpdateState(UIHandler::StateId::State_Loading_Close);
        if (Sequence::getPtr()->readBoxState())
            Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_Pierce);
    }
    else if(result == Sequence::SequenceResult::Result_Box_Valid)
    {
        UIHandler::GoPage(UIHandler::PageId::Page_Main_BoxReady);
    }
    else if(result == Sequence::SequenceResult::Result_Box_Invalid)
    {
    }
}

void Idle::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == timerid){
        if (Sequence::getPtr()->isIdle())
            Sequence::getPtr()->actionDo("Query",3,0,0,0);
    }
    else
        killTimer(e->timerId());
}
