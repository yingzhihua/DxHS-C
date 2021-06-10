#include "startup.h"
#include "ui_startup.h"

#include "module/exglobal.h"
#include "module/uihandler.h"

static Sequence *sequence = nullptr;
Startup::Startup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Startup)
{
    ui->setupUi(this);
    QImage image(QString(":/images/startup_logo.png"));
    ui->label->setGeometry((1920-image.width())/2,(900-image.height())/2,image.width(),image.height());
    ui->label->setPixmap(QPixmap::fromImage(image));
    sequence = Sequence::getPtr();
    connect(sequence,&Sequence::sequenceFinish,this,&Startup::sequenceFinish);

    sequence->sequenceInit();
    sequence->actionDo("Query",3,0,0,0);
}

Startup::~Startup()
{
    Sequence::getPtr()->disconnect(this);
    delete ui;
}

void Startup::sequenceFinish(Sequence::SequenceResult result)
{
    qDebug()<<"startup,sequenceFinish,result="<<result;

    if (result == Sequence::SequenceResult::Result_SelfCheck_ok)
    {
        //changeTimer.stop();
        //pagequit = true;
        //mainView.pop();
        //mainView.push("qrc:/HomeUI/Login.qml",StackView.Immediate);
        UIHandler::GoPage(UIHandler::PageId::Page_Main_Login);
    }
    else if (result == Sequence::SequenceResult::Result_OpenBox_ok)
    {
        //openDoor.playing = false
        //openDoor.enabled = true
    }
    else if(result == Sequence::SequenceResult::Result_CloseBox_ok)
    {
        //openDoor.playing = false
        //openDoor.enabled = true

        Sequence::getPtr()->actionDo("Query",3,0,0,0);
    }
    else if (result == Sequence::SequenceResult::Result_Simple_ok){
        if (Sequence::getPtr()->readBoxState()){
            //Sequence.setTitle("startup_error")
            //boxTips.text = qsTr("请取出试剂盒 然后关闭舱门")
        }
        else if (Sequence::getPtr()->doorError())
        {
            //Sequence.setTitle("startup_error")
            //boxTips.text = qsTr("请恢复舱门")
        }
        else if (Sequence::getPtr()->readDoorState()){
            //Sequence.setTitle("startup_error")
            //boxTips.text = qsTr("请关闭舱门")
        }

        if (Sequence::getPtr()->doorError() || (ExGlobal::projectMode() != 0 && (Sequence::getPtr()->readBoxState() || Sequence::getPtr()->readDoorState()))){
            //appicon.visible = false;
            //openDoor.visible = true;
            //boxTips.visible = true;
        }
        else {
            //appicon.visible = true;
            //openDoor.visible = false;
            //boxTips.visible = false;
            //Sequence.setTitle("startup")
            Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_SelfCheck);
            //changeTimer.start();
        }
    }
}
