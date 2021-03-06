#include "startup.h"
#include "ui_startup.h"

#include "module/exglobal.h"
#include "module/uihandler.h"

static int timerid = 0;
Startup::Startup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Startup)
{
    ui->setupUi(this);
    QImage image(QString(":/images/startup_logo.png"));
    ui->Home_Startup_lbLogo->setGeometry((1920-image.width())/2,(900-image.height())/2,image.width(),image.height());
    ui->Home_Startup_lbLogo->setPixmap(QPixmap::fromImage(image));
    ui->Home_Startup_btOpenDoor->setGeometry((1920-800)/2,(900-400)/2,800,400);

    Sequence::getPtr()->sequenceInit();    
}

Startup::~Startup()
{    
    delete ui;
}

void Startup::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    qDebug()<<"startup \n\n\n";
    connect(Sequence::getPtr(),&Sequence::sequenceFinish,this,&Startup::sequenceFinish);
    Sequence::getPtr()->actionDo("Query",3,0,0,0);
}

void Startup::hideEvent(QHideEvent *event){
    Q_UNUSED(event);
    if (timerid != 0){
        killTimer(timerid);
        timerid = 0;
    }
    Sequence::getPtr()->disconnect(this);
}

void Startup::sequenceFinish(Sequence::SequenceResult result)
{
    qDebug()<<"startup,sequenceFinish,result="<<result;

    if (result == Sequence::SequenceResult::Result_SelfCheck_ok)
    {        
        UIHandler::GoPage(UIHandler::PageId::Page_Main_Login);
    }
    else if (result == Sequence::SequenceResult::Result_OpenBox_ok)
    {
        UIHandler::UpdateState(UIHandler::StateId::State_Loading_Close);
    }
    else if(result == Sequence::SequenceResult::Result_CloseBox_ok)
    {
        UIHandler::UpdateState(UIHandler::StateId::State_Loading_Close);
        Sequence::getPtr()->actionDo("Query",3,0,0,0);
    }
    else if (result == Sequence::SequenceResult::Result_Simple_ok){
        if (Sequence::getPtr()->readBoxState()){            
            ui->Home_Startup_btOpenDoor->setText(tr("请取出试剂盒 然后关闭舱门"));
        }
        else if (Sequence::getPtr()->doorError())
        {
            ui->Home_Startup_btOpenDoor->setText(tr("请恢复舱门"));
        }
        else if (Sequence::getPtr()->readDoorState()){
            ui->Home_Startup_btOpenDoor->setText(tr("请关闭舱门"));
        }

        qDebug()<<"projectMode="<<ExGlobal::projectMode()<<"BoxState="<<Sequence::getPtr()->readBoxState()<<Sequence::getPtr()->readDoorState();
        if (Sequence::getPtr()->doorError() || (ExGlobal::projectMode() != 0 && (Sequence::getPtr()->readBoxState() || Sequence::getPtr()->readDoorState()))){
            ui->Home_Startup_lbLogo->setVisible(false);
            ui->Home_Startup_btOpenDoor->setVisible(true);
        }
        else {            
            ui->Home_Startup_lbLogo->setVisible(true);
            ui->Home_Startup_btOpenDoor->setVisible(false);
            Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_SelfCheck);            
            timerid = startTimer(500);
        }
    }
}

void Startup::on_Home_Startup_btOpenDoor_clicked()
{
    if (Sequence::getPtr()->doorError())
    {
        if (Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_ErrOpenBox))
        {
            UIHandler::UpdateState(UIHandler::StateId::State_Loading_Open);
        }
    }
    else if (Sequence::getPtr()->readDoorState() == false)
    {
        if (Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_OpenBox))
        {
            UIHandler::UpdateState(UIHandler::StateId::State_Loading_Open);
        }
    }
    else
    {
        if (Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_CloseBox))
        {
            UIHandler::UpdateState(UIHandler::StateId::State_Loading_Open);
        }
    }
}

static int titlechangecount = 0;
void Startup::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == timerid){
        QString header = tr("系统启动");
        if (titlechangecount == 1)
            header += ".";
        else if (titlechangecount == 2)
            header += "..";
        else if (titlechangecount == 3)
            header += "...";
        else if (titlechangecount == 4)
            header += "....";
        else
            header += ".....";
        if (++titlechangecount > 5)
            titlechangecount = 0;
        Sequence::getPtr()->changeTitle(header);
    }
}
