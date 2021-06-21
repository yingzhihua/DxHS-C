#include "startup.h"
#include "ui_startup.h"

#include "module/exglobal.h"
#include "module/uihandler.h"

Startup::Startup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Startup)
{
    ui->setupUi(this);
    QImage image(QString(":/images/startup_logo.png"));
    ui->Home_Startup_lbLogo->setGeometry((1920-image.width())/2,(900-image.height())/2,image.width(),image.height());
    ui->Home_Startup_lbLogo->setPixmap(QPixmap::fromImage(image));
    ui->Home_Startup_btOpenDoor->setGeometry((1920-500)/2,(900-400)/2,500,400);    

    connect(Sequence::getPtr(),&Sequence::sequenceFinish,this,&Startup::sequenceFinish);

    Sequence::getPtr()->sequenceInit();
    Sequence::getPtr()->actionDo("Query",3,0,0,0);
}

Startup::~Startup()
{    
    delete ui;
}

void Startup::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    connect(Sequence::getPtr(),&Sequence::sequenceFinish,this,&Startup::sequenceFinish);
}

void Startup::hideEvent(QHideEvent *event){
    Q_UNUSED(event);
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
            //Sequence.setTitle("startup_error")
            ui->Home_Startup_btOpenDoor->setText(tr("请取出试剂盒 然后关闭舱门"));
        }
        else if (Sequence::getPtr()->doorError())
        {
            //Sequence.setTitle("startup_error")
            ui->Home_Startup_btOpenDoor->setText(tr("请恢复舱门"));
            //boxTips.text = qsTr("请恢复舱门")
        }
        else if (Sequence::getPtr()->readDoorState()){
            //Sequence.setTitle("startup_error")
            ui->Home_Startup_btOpenDoor->setText(tr("请关闭舱门"));
            //boxTips.text = qsTr("请关闭舱门")
        }

        if (Sequence::getPtr()->doorError() || (ExGlobal::projectMode() != 0 && (Sequence::getPtr()->readBoxState() || Sequence::getPtr()->readDoorState()))){            
            ui->Home_Startup_lbLogo->setVisible(false);
            ui->Home_Startup_btOpenDoor->setVisible(true);
        }
        else {            
            ui->Home_Startup_lbLogo->setVisible(true);
            ui->Home_Startup_btOpenDoor->setVisible(false);
            Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_SelfCheck);            
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
