#include "sequence.h"
#include <QDebug>

#include "exglobal.h"
#include "actionparser.h"
#include "imageanalysis.h"
#include "sqlitemgr.h"
#include "datahandler.h"
#include "uihandler.h"

#define AUTOFOCUS_MIN 2600
#define AUTOFOCUS_MAX 7000
#define AUTOFOCUS_STEP 100
#define AUTOFOCUS_MAXOFFSET 6

static int autoFocus_CurrPoint = 0;
static int autoFocus_ClarityPoint = 0;
static double autoFocus_ClarityValue = 0;
static int autoFocus_JumpStep = 0;
static bool autoFocus_dec = false;

static QDomDocument doc;
static char currOrder = 0;
static int currCameraCaptureType = 0;
static int currCameraCycle = 0;
static QDateTime testStartTime;
static QMetaEnum metaEnum = QMetaEnum::fromType<Sequence::SequenceId>();

static int ErrOpenDoorCount = 0;
#define ERROPENDOOR_MAXLENGTH 10000
static Sequence *sequence = nullptr;

static bool hasTime0119 = false;


Sequence *Sequence::getPtr()
{
    if (sequence == nullptr)
        sequence = new Sequence();
    return sequence;
}

Sequence::Sequence(QObject *parent) : QObject(parent)
{
    ExGlobal::bufrgb = (unsigned char *)malloc(2592 * 1944 * 3);
    ExGlobal::hbufrgb = (ExGlobal::bufrgb) + (1296*1944*3);

    imageAna = new ImageAnalysis();
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setSingleShot(true);
    waitNextSequence = new QTimer(this);
    waitNextSequence->setSingleShot(true);

    connect(timer,SIGNAL(timeout()),this,SLOT(SequenceTimeout()));
    connect(waitNextSequence,SIGNAL(timeout()),this,SLOT(WaitSequenceTimeout()));

    testSecondTime = new QTimer(this);
    testSecondTime->setSingleShot(false);
    connect(testSecondTime,SIGNAL(timeout()),this,SLOT(TestSecondTimeout()));

    serialMgr = new SerialMgr();
    connect(serialMgr,&SerialMgr::finishAction,this,&Sequence::ActionFinish);
    connect(serialMgr,&SerialMgr::errAction,this,&Sequence::errFinish);
    connect(serialMgr,&SerialMgr::errOccur,this,&Sequence::errReceive);


    cvcap = new cvCapture();

    //imageCapture = new ImageCapture();
    //connect(imageCapture,&ImageCapture::finishCapture,this,&Sequence::ActionFinish);
    //connect(imageCapture,&ImageCapture::reView,this,&Sequence::CameraView);

    camera = new TCamera();
    connect(camera,&TCamera::finishCapture,this,&Sequence::ActionFinish);
    connect(camera,&TCamera::reView,this,&Sequence::CameraView);

    testMgr = new TestMgr();

    qr = new QRcoder();
    connect(qr,&QRcoder::finishQRcode,this,&Sequence::ActionFinish);

    printer = new printmgr();
    connect(printer,&printmgr::finishPrint,this,&Sequence::PrintFinish);

    currSequenceId = SequenceId::Sequence_Idle;
    bCannelSequence = false;

    bValidBox = false;

    stage = StageState::Stage_selfcheck;
    //actionDo("Sensor",0,0,0);
    serialMgr->serialWrite(ActionParser::ParamToByte("AutoData",1,0,0,0));
//    serialMgr->serialWrite(ActionParser::ParamToByte("Door",3,12000,0,0));
    serialMgr->serialWrite(ActionParser::ParamToByte("Query",5,19,0,0));

    ImageAnalysis::FocusSet(ExGlobal::FocusX,ExGlobal::FocusY,ExGlobal::FocusWidth,ExGlobal::FocusHeight);
    ImageAnalysis::LightSet(ExGlobal::LightCX,ExGlobal::LightCY,ExGlobal::LightR);

    udpServer = new UdpServer;
}

bool Sequence::sequenceInit(){
    if (!ReadTestProcess(QCoreApplication::applicationDirPath()+"/FLASHDXcn"))
        return false;
    ExGlobal::pTestModel->InitTest();
    ExGlobal::pLogModel->InitLog();
    if (camera->cameraType == CAMERA_EMPTY)
    {
        errReceive(ERROR_CODE_CAM_CONNECT);
        return false;
    }
    return true;
}


QStringList Sequence::getTestList(){
    QStringList test;
    QDomElement root = doc.documentElement();
    OnePointPanelIndex =-1;
    for (QDomElement e = root.firstChildElement("Panel"); !e.isNull(); e = e.nextSiblingElement("Panel"))
    {
        QString panelCode = e.attribute("Panel_ID");
        if (panelCode.length() > 4 && panelCode.startsWith("90"))
            continue;
        test<<e.attribute("Panel_c");
        if (e.attribute("Panel_ID") == ExGlobal::OnePointPanelCode)
            OnePointPanelIndex = test.size()-1;
    }
    return test;
}

void Sequence::setOnePointPanelIndex(int index){
    QStringList test;
    QDomElement root = doc.documentElement();
    OnePointPanelIndex =-1;
    qDebug()<<"setOnePointPanelIndex"<<index;
    for (QDomElement e = root.firstChildElement("Panel"); !e.isNull(); e = e.nextSiblingElement("Panel"))
    {
        QString panelCode = e.attribute("Panel_ID");
        if (!panelCode.startsWith("3"))
            continue;
        test<<e.attribute("Panel_c");
        qDebug()<<"test size"<<test.size()<<e.attribute("Panel_ID");
        if (test.size() == index+1)
        {
            ExGlobal::OnePointPanelCode = e.attribute("Panel_ID");
            break;
        }
    }
}

void Sequence::sequenceSetPanel(QString panelName)
{
    QDomElement root = doc.documentElement();

    for (QDomElement e = root.firstChildElement("Panel"); !e.isNull(); e = e.nextSiblingElement("Panel"))
    {
        if (e.attribute("Panel_c")==panelName || e.attribute("Panel_e")==panelName || e.attribute("Panel_ID")==panelName)
        {
            ExGlobal::setPanelName(e.attribute("Panel_c"));
            ExGlobal::setPanelCode(e.attribute("Test_ID"));
            ExGlobal::setPanelId(e.attribute("Panel_ID"));
            if (e.hasAttribute("Layout_ID"))
                ExGlobal::setReagentBox(e.attribute("Layout_ID"));
            emit panelNameChanged();
            break;
        }
    }
}

QString Sequence::getCurrTestTime(){
    return QDateTime::fromMSecsSinceEpoch(QDateTime::currentDateTime().toMSecsSinceEpoch() - testStartTime.toMSecsSinceEpoch()).toUTC().toString("hh:mm:ss");
}

bool Sequence::sequenceDo(SequenceId id)
{
    //QMetaEnum metaEnum = QMetaEnum::fromType<SequenceId>();

    qDebug()<<"sequenceDo,currSequenceId:"<<currSequenceId<<id;
    //emit titleNotify(200,"正在测试，预计剩余31分钟");
    //sequenceFinish(SequenceResult::Result_Test_finish);
    Log::LogWithTime(QString("sequenceDo:%1,currSequenceId:%2").arg(metaEnum.valueToKey(id)).arg(metaEnum.valueToKey(currSequenceId)));
    if (currSequenceId != SequenceId::Sequence_Idle)
    {
        this->nextSequenceId = id;
        waitCount = 0;
        waitNextSequence->start(200);
        return false;
    }
    this->currSequenceId = id;
    lampDo(LampState::Lamp_Light_green);
    qDebug()<<"Lamp1";
    QDomElement root = doc.documentElement();
    if (id == SequenceId::Sequence_Test)
    {
        /*
        if (ExGlobal::diskCheck() == 2)
        {
            errReceive(ERROR_CODE_DISK_FULL);
            return false;
        }
        */
        for (sequenceAction = root.firstChildElement("PanelTest"); !sequenceAction.isNull(); sequenceAction = sequenceAction.nextSiblingElement("PanelTest"))
        {
            if (sequenceAction.attribute("PanelCode")==ExGlobal::panelCode())
            {
                testStartTime = QDateTime::currentDateTime();
                QString current_time_str = testStartTime.toString("yyyyMMdd_hhmmss");
                Log::setDir(QCoreApplication::applicationDirPath()+"/"+current_time_str);
                camera->openCamera();
                //ExGlobal::setPanelName(sequenceAction.attribute("Panel_c"));
                imageAna->SetMask(ExGlobal::getReagentBox(ExGlobal::reagentBox()),0);
                testMgr->TestCreate(ExGlobal::boxSerial());
                bFocused = false;
                dryMeanValue = 0;
                fillMeanValue = 0;
                nTestSecond = 0;
                masked = false;
                testSecondTime->start(1000);
                break;
            }
        }
    }
    else if(id == SequenceId::Sequence_OpenBox)
    {
        //sequenceAction = root.firstChildElement("OpenDoor");
        SwitchDoor(false);
        return true;
    }
    else if(id == SequenceId::Sequence_ErrOpenBox)
    {
        SwitchDoor(true);
        return true;
    }
    else if(id == SequenceId::Sequence_CloseBox){
        //sequenceAction = root.firstChildElement("CloseDoor");
        SwitchDoor(false);
        return true;
    }
    else if(id == SequenceId::Sequence_DoorToWork){
        DoorToWork(true);
        return true;
    }
    else if(id == SequenceId::Sequence_DoorFromWork){
        DoorToWork(false);
        return true;
    }
    else if(id == SequenceId::Sequence_SelfCheck){
        sequenceAction = root.firstChildElement("SelfCheck");
    }
    else if(id == SequenceId::Sequence_CannelTest){
        sequenceAction = root.firstChildElement("StopTest");
    }
    else if(id == SequenceId::Sequence_QrDecode)
    {
        qrDect();
        return true;
    }
    else if(id == SequenceId::Sequence_Pierce)
    {
        PierceDect();
        return true;
    }

    if (sequenceAction.isNull())
        return false;

    if (!FormatAction())
        return false;

    bFinishAction = false;
    durationState = TimeState::idle;
    bCannelSequence = false;

    FindAction(false);
    return true;
}

bool Sequence::actionDo(QString device, int value, int param1, int param2, int param3)
{
    qDebug()<<"actionDo:"<<device<<",currSequenceId:"<<currSequenceId;
    if (ExGlobal::isDebug())
        return false;
    Log::LogWithTime(QString("actionDo,currSequenceId:%1,device:%2").arg(metaEnum.valueToKey(currSequenceId)).arg(device));
    if (currSequenceId != SequenceId::Sequence_Idle)
        return false;
    currSequenceId = SequenceId::Sequence_SimpleAction;
    if (device == "Door" && value == 1){
        param1 = ExGlobal::DoorOut;
    }
    QByteArray send = ActionParser::ParamToByte(device,value,param1,param2,param3);
    currOrder = send[7];
    serialMgr->serialWrite(send);
    return true;
}

bool Sequence::dirctAction(QString device, int value, int param1, int param2, int param3){
    Log::LogWithTime(QString("dirctAction,device:%1,value:%2,param1:%3,param2:%4,param3:%5").arg(device).arg(value).arg(param1).arg(param2).arg(param3));
    QByteArray send = ActionParser::ParamToByte(device,value,param1,param2,param3);
    currOrder = send[7];
    serialMgr->serialWrite(send);
    return true;
}

void Sequence::sequenceCancel()
{
    qDebug()<<"sequenceCancel:"<<",currSequenceId:"<<currSequenceId<<",durationState:"<<durationState<<",bFinishAction:"<<bFinishAction;

    if (!isTesting()&&!isLoopTesting())
        return;

    if(durationState == TimeState::running)
        timer->stop();

    if (bFinishAction) {
        currSequenceId = SequenceId::Sequence_Idle;
        sequenceDo(SequenceId::Sequence_CannelTest);
    }
    else {
        bCannelSequence = true;
    }
}

void Sequence::sequenceContinue(){
    testSecondTime->start(1000);
    SequenceTimeout();
}

void Sequence::SequenceTimeout()
{
    qDebug()<<"Action Duration finish";
    durationState = TimeState::idle;
    if (bFinishAction)
        FindAction(true);

}

void Sequence::WaitSequenceTimeout()
{
    qDebug()<<"WaitSequenceTimeout,count:"<<waitCount;
    waitCount++;
    if (waitCount < 30)
    {
        if (currSequenceId != SequenceId::Sequence_Idle)
               waitNextSequence->start(400);
         else
              sequenceDo(nextSequenceId);
    }
}

void Sequence::TestSecondTimeout(){
    qDebug()<<"TestSecondTimeout,nPreTime="<<nPreTime<<"nDuration="<<nDuration<<"TotalTime="
           <<sequenceAction.attribute("Duration")<<"nTestSecond="<<nTestSecond;
    Log::LogByFile("ProgressBar.txt",QString("TestSecondTimeout,nPreTime=%1,nDuration=%2,TotalTime=%3,nTestSecond=%4")
                   .arg(nPreTime).arg(nDuration).arg(sequenceAction.attribute("Duration")).arg(nTestSecond));
    if ((nPreTime + nDuration)/1000 > nTestSecond)
        nTestSecond++;
    int remain = sequenceAction.attribute("Duration").toInt()/1000 - nTestSecond;
    title = QString(tr("正在测试，预计剩余"))+QString("%1").arg(remain/60)+QString(tr("分"))+QString("%2").arg(remain%60)+QString(tr("秒"));
    if (currSequenceId == SequenceId::Sequence_CannelTest)
        title = QString(tr("正在取消测试"));
    if (isTesting())
        emit titleNotify((nTestSecond*1000)/(sequenceAction.attribute("Duration").toInt()/1000)+100,title);
}

void Sequence::ActionFinish(QByteArray data)
{
    qDebug()<<"Sequence ActionFinish:"<<data.toHex(' ');
    qDebug()<<"currSequenceId:"<<currSequenceId;
    if(bAutoFocus)
        Log::LogByFile("Focus.txt",data.toHex(' '));
    if (data[7] == '\x72'){
        //if (currSequenceId != SequenceId::Sequence_Test){
        if (currSequenceId == SequenceId::Sequence_Idle){
            /*
            if (bDoorState == false)//close
                sequenceDo(SequenceId::Sequence_OpenBox);
            else
                sequenceDo(SequenceId::Sequence_CloseBox);
                */
            emit doorKeyPress();
        }
        return;
    }
    else if(data.length()>11 && data[7] == '\x82' && data[1] == '\x02'){
        updateParam(data);
    }
    else if(data.length()>15 && data[7] == '\x61' && data[12] == '\x13' && data[15] != '\xFF'){
        hasTime0119 = true;
    }

    if (currOrder != data[7])
        return;

    if (data.length()>11 && data[0] == '\xaa' && data[data.length()-1]=='\x55')
    {
        if (data[1] == '\x02')  //temp board
        {
            if (data[7] == '\x62'){
                ExGlobal::settempversion(data.mid(13,5).data());
            }
            else if(data[7] == '\x20'){
            }
            else if(data[7] == '\x1F'){
                if (data[12] == '\x00')
                    fan1SetSpeed((data[13]<<8)+data[14]);
                else if (data[12] == '\x01')
                    fan1SetSpeed((data[13]<<8)+data[14]);
                else if (data[12] == '\x02')
                    fan1SetSpeed((data[13]<<8)+data[14]);
                int fan = data[12];
                int fanspeed = (data[13]<<8)+data[14];
                if (isLoopTesting())
                    Log::LogByFile("LoopTest.txt",QString("风扇:%1,转速：%2").arg(fan).arg(fanspeed));
            }
            else if(data[7] == '\x25' && data.length() == 40){
                QString logstr;
                for (int i = 0; i < 13; i++){
                    int temp1 = (data[2*i+13]<<8) + data[2*i+14];
                    double ftemp = (double)temp1/100;
                    if (i == 0)
                        logstr += QString("\n后帕尔贴A温度：\t%1\n").arg(ftemp);
                    else if (i == 1)
                        logstr += QString("后帕尔贴B温度：\t%1\n").arg(ftemp);
                    else if (i == 2)
                        logstr += QString("前帕尔贴A温度：\t%1\n").arg(ftemp);
                    else if (i == 3)
                        logstr += QString("前帕尔贴B温度：\t%1\n").arg(ftemp);
                    else if (i == 4)
                        logstr += QString("裂解A温度：\t%1\n").arg(ftemp);
                    else if (i == 5)
                        logstr += QString("裂解B温度：\t%1\n").arg(ftemp);
                    else if (i == 6)
                        logstr += QString("后置散热器A温度：\t%1\n").arg(ftemp);
                    else if (i == 7)
                        logstr += QString("后置散热器B温度：\t%1\n").arg(ftemp);
                    else if (i == 8)
                        logstr += QString("前置散热器A温度：\t%1\n").arg(ftemp);
                    else if (i == 9)
                        logstr += QString("前置散热器B温度：\t%1\n").arg(ftemp);
                    else if (i == 10)
                        logstr += QString("环境温度：\t1%1\n").arg(ftemp);
                    else if (i == 11)
                        logstr += QString("预设1温度：\t%1\n").arg(ftemp);
                    else if (i == 12)
                        logstr += QString("预设2温度：\t%1\n").arg(ftemp);
                }
                Log::LogWithTime(logstr);
            }
        }
        else if (data[1] == '\x01') //drive board
        {
            if (data[7] == '\x62')
                ExGlobal::setctrlversion(data.mid(13,5).data());
            else if (data[7] == '\x32')
                setSenorState(data[12],data[13]);
            else if(bAutoFocus == true && data[7] == '\x70' && data.length()>12 && (data[12] == '\x2a'||data[12] == '\x2b'))
            {
                autoFocus_JumpStep = 1;
                Log::LogByFile("Focus.txt",QString("ActionFinish,move step."));
                return;
            }
        }
        else if(data[1] == '\x03')//CameraCapture
        {
            qDebug()<<"CameraCapture ActionFinish,type="<<currCameraCaptureType;
            if (data[7] == '\xa0' && data[10] == '\x00'){
                if (currCameraCaptureType == 5){
                    /*
                    if (currCameraCycle < 2)
                        imageAna->FirstImage(imageCapture->getyData(),imageCapture->imagetype);
                    else
                        imageAna->AddImage(imageCapture->getyData(),imageCapture->imagetype);
                    //*/
                    //*
                    if (!masked)
                    {
                        imageAna->MaskHandle(camera->getyData(),camera->getImageType(),Log::getDir()+QString("/Fill.tif"));
                        masked = true;
                    }
                    imageAna->AddImage(camera->getyData(),camera->getImageType(),Log::getDir()+QString("/cycle%1.tif").arg(currCameraCycle,2,10,QChar('0')));
                    //*/

                    //imageProvider->anaMainImg = imageAna->getMainImg(0,1);
                    emit callQmlRefeshAnaMainImg();
                    QVector<int> item = imageAna->getItem();
                    QVector<int> value = imageAna->getValue();
                    QVector<int> posIndex = imageAna->getIndex();
                    QVector<QVector<int>> PosValue = imageAna->getPosValueArr();
                    emit callQmlRefeshData(currCameraCycle,item,value);

                    if (ExGlobal::panelCode().startsWith("3")){
                        testMgr->InsertData(0,2,currCameraCycle,(int)(imageAna->GetLLight(camera->getyData(),camera->getImageType())*100));
                    }
                    else {
                        SqliteMgr::StartTransations();
                        /*
                        for(int i = 0; i < item.size(); i++){
                            testMgr->InsertData(posIndex[i],item[i],currCameraCycle,value[i]);
                        }
                        */
                        for (int i = 0; i < PosValue.size(); i++)
                            testMgr->InsertData(currCameraCycle,PosValue[i]);

                        SqliteMgr::EndTransations();
                    }
                }
                else if(currCameraCaptureType == 4){
                    imageAna->SetBlackImage(camera->getyData(),camera->getImageType());
                }
                else if(currCameraCaptureType == 1){
                    dryMeanValue = imageAna->GetMeanLight(camera->getyData(),camera->getImageType());
                    //Log::LogTime(QString("Dry Mean Value:%1").arg(dryMeanValue));
                }
                else if(currCameraCaptureType == 3){
                    fillMeanValue = imageAna->GetMeanLight(camera->getyData(),camera->getImageType());
                    //Log::LogTime(QString("Dry Mean Value:%1,Fill Mean Value:%2").arg(dryMeanValue).arg(fillMeanValue));
                    if (ExGlobal::panelCode().startsWith("3"))
                    {
                        //注册检测试湿片比干片更亮,暂时定标准>1.5
                        Log::LogCData(QString("Fill Value/Dry Value = %1/%2 = %3 (reference value > %4/10)").arg(fillMeanValue).arg(dryMeanValue).arg(fillMeanValue/dryMeanValue).arg(15));
                        if (dryMeanValue > 3 && fillMeanValue > 3 && fillMeanValue*10/dryMeanValue < 15)
                            emit sequenceFinish(SequenceResult::Result_Test_DryFillErr);
                    }
                    else{
                        Log::LogCData(QString("Dry Value/Fill Value = %1/%2 = %3 (reference value > %4/10)").arg(dryMeanValue).arg(fillMeanValue).arg(dryMeanValue/fillMeanValue).arg(ExGlobal::DryWet));
                        if (dryMeanValue > 3 && fillMeanValue > 3 && dryMeanValue*100/fillMeanValue < ExGlobal::DryWet)
                            emit sequenceFinish(SequenceResult::Result_Test_DryFillErr);
                    }
                    imageAna->MaskHandle(camera->getyData(),camera->getImageType(),Log::getDir()+QString("/Fill.tif"));
                    masked = true;
                }
            }
            else if(data[7] == '\xa0' && data[10] != '\x00'){
                //errReceive(0x300 + data[10]);
                errReceive(ERROR_CODE_CAM_CONNECT);
                return;
            }

            currCameraCaptureType = 0;
        }
    }
    else if (data[1] == '\x04'){
        if (data[7] == '\xB0'){     //二维码识别
            //if (data[2] == '\x01' || data[2] == '\x03')
                emit callQmlRefeshQrImg();
            data.remove(0,10);
            emit qrDecode(data.data());
        }
        else if(data[7] == '\xB1'){ //刺穿识别 data[2]=1:识别出二维码;data[3]=1:有试剂液体
            if (ExGlobal::projectMode() == 3){
                //if (data[2] == '\x0' && decodeQr(data.mid(10)) == 2)
                /*
                if (data[2] != '\x0')
                {
                    decodeQr(data.mid(10));
                    //ExGlobal::setReagentBox("205");
                }
                else
                //*/
                {
                    sequenceSetPanel(ExGlobal::DemoPanelCode);
                    //ExGlobal::setReagentBox("205");
                    ExGlobal::setReagentBox("301");
                    ExGlobal::setBoxSerial(QString("Lot# 20001"));
                    ExGlobal::validDateTime = QDateTime::currentDateTime().addDays(90);
                }
                emit sequenceFinish(SequenceResult::Result_Box_Valid);
            }
            else if (ExGlobal::projectMode() == 4){
                if (data[2] != '\x0')
                {
                    decodeQr(data.mid(10));
                    //ExGlobal::setReagentBox("206");
                }
                else {
                    sequenceSetPanel(ExGlobal::OnePointPanelCode);
                    ExGlobal::setReagentBox("206");
                    ExGlobal::setBoxSerial(QString("Lot# 30002"));
                    ExGlobal::validDateTime = QDateTime::currentDateTime().addDays(90);
                }

                emit sequenceFinish(SequenceResult::Result_Box_Valid);
            }
            else if(data[2] == '\x0')
            {
                boxparam = 4;
                emit sequenceFinish(SequenceResult::Result_Box_Invalid);
            }
            else
            {
                int decodeResult = decodeQr(data.mid(10));
                if (decodeResult == 1){
                    boxparam = 5;
                    emit sequenceFinish(SequenceResult::Result_Box_Invalid);
                }
                else if(data[3] == '\x0')
                {
                    boxparam = 2;
                    qDebug()<<"Result_Box_Invalid";
                    emit sequenceFinish(SequenceResult::Result_Box_Invalid);
                }
                else
                {
                    qDebug()<<"Result_Box_Valid";
                    emit sequenceFinish(SequenceResult::Result_Box_Valid);
                }
            }
        }
    }

    if (bCannelSequence)
    {
        currSequenceId = SequenceId::Sequence_Idle;
        sequenceDo(SequenceId::Sequence_CannelTest);
        return;
    }

    if (currSequenceId == SequenceId::Sequence_SimpleAction)
    {
        currSequenceId = SequenceId::Sequence_Idle;
        emit sequenceFinish(SequenceResult::Result_Simple_ok);
    }
    else if (currSequenceId == SequenceId::Sequence_QrDecode || currSequenceId == SequenceId::Sequence_Pierce
             ||currSequenceId == SequenceId::Sequence_OpenBox || currSequenceId == SequenceId::Sequence_CloseBox
             ||currSequenceId == SequenceId::Sequence_DoorToWork ||currSequenceId == SequenceId::Sequence_DoorFromWork
             ||currSequenceId == SequenceId::Sequence_ErrOpenBox){
        if (!listNextAction(false))
        {
            FinishSequence();
        }
    }
    else{
        bFinishAction = true;
        //qDebug()<<"Sequence ActionFinish,durationState:"<<durationState;


        Log::LogTime(QString("Action finish,duration state:%1").arg((int)durationState));

        if (durationState == TimeState::idle)
            FindAction(true);
        else if(durationState == TimeState::wait)
        {
            durationState = TimeState::running;
            Log::LogTime(QString("Start timer:%1").arg(nDuration));
            timer->start(nDuration);
        }

        if (durationState == TimeState::running && currSequenceId == SequenceId::Sequence_Test && bFocused == false && nDuration >= 200000)
        {
            if (ExGlobal::projectMode() == 1)
            {
                Log::LogByFile("Focus.txt",QString("ActionFinish,nDuration:%1").arg(nDuration));
                autoFocus();
            }
        }
    }
}

void Sequence::errFinish(QByteArray data){
    qDebug()<<"Sequence errFinish:"<<data.toHex(' ');
    QString errStr = tr("未知错误！")+data.toHex(' ');
    int errCode = 0;

    if (data.length()>11 && data[0] == '\xaa' && data[data.length()-1]=='\x55'){
        if (data[1] == '\x02'){
            if (data[7] == '\x89'){
                int temp1 = (data[12]<<8)+data[13];
                int temp2 = (data[14]<<8)+data[15];
                int chan = data[11];
             //   errStr = QString("通道%1两传感器温差过大，一传感器温度：%2，一传感器温度：%3").arg(chan).arg(temp1).arg(temp2);
                errStr = QString(tr("通道"))+QString("%1").arg(chan)+QString(tr("两传感器温差过大，一传感器温度："))+QString("%2").arg(temp1)+QString(tr("，一传感器温度："))+QString("%3").arg(temp2);

                errCode = ERROR_CODE_TEMP_DIFF;
            }
            else if(data[7] == '\x24')
            {
                errStr = tr("温度错误！");
                errCode = ERROR_CODE_TEMP_ERR;
            }

        }
        else if(data[1] == '\x01' && data.length() > 15){
            if (data[13] == '\x75' && data[14] == '\x4B'){
                errStr = tr("在电机动作未完车时，新的电机被启动！");
                errCode = 0x794B;
            }
            else if (data[13] == '\x75' && data[14] == '\x55'){
                errStr = tr("新时序与上个时序存在时间冲突，驱动板停止运行！");
                errCode = 0x7555;
            }
            else if (data[13] == '\x75' && data[14] == '\x5F'){
                errStr = tr("在电机动作未完成时，时序已退出！");
                errCode = 0x755F;
            }
            else if (data[13] == '\x79' && data[14] == '\x18'){
                errStr = tr("电机运动超时！");
                errCode = 0x7918;
            }
            else if (data[13] == '\x79' && data[14] == '\x19'){
                errStr = tr("注射电机复位失败！");
                errCode = 0x7919;
            }
            else if (data[13] == '\x79' && data[14] == '\x1a'){
                errStr = tr("注射电机光耦异常！");
                errCode = 0x791a;
            }
            else if (data[13] == '\x79' && data[14] == '\x1b'){
                errStr = tr("注射电机运动超速！");
                errCode = 0x791b;
            }
            else if (data[13] == '\x79' && data[14] == '\x1c'){
                errStr = tr("阀一电机复位失败！");
                errCode = 0x791c;
            }
            else if (data[13] == '\x79' && data[14] == '\x1d'){
                errStr = tr("阀一光耦异常！");
                errCode = 0x791d;
            }
            else if (data[13] == '\x79' && data[14] == '\x1e'){
                errStr = tr("阀一电机运动超时！");
                errCode = 0x791e;
            }
            else if (data[13] == '\x79' && data[14] == '\x1f'){
                errStr = tr("阀二电机复位失败！");
                errCode = 0x791f;
            }
            else if (data[13] == '\x79' && data[14] == '\x20'){
                errStr = tr("阀二光耦异常！");
                errCode = 0x7920;
            }
            else if (data[13] == '\x79' && data[14] == '\x21'){
                errStr = tr("阀二电机运动超时！");
                errCode = 0x7921;
            }
            else if (data[13] == '\x79' && data[14] == '\x22'){
                errStr = tr("阀三电机复位失败！");
                errCode = 0x7922;
            }
            else if (data[13] == '\x79' && data[14] == '\x23'){
                errStr = tr("阀三光耦异常！");
                errCode = 0x7923;
            }
            else if (data[13] == '\x79' && data[14] == '\x24'){
                errStr = tr("阀三电机运动超时！");
                errCode = 0x7924;
            }
            else if (data[13] == '\x79' && data[14] == '\x25'){
                errStr = tr("刺破电机复位失败！");
                errCode = 0x7925;
            }
            else if (data[13] == '\x79' && data[14] == '\x26'){
                errStr = tr("刺破光耦异常！");
                errCode = 0x7926;
            }
            else if (data[13] == '\x79' && data[14] == '\x27'){
                errStr = tr("刺破电机运动超时！");
                errCode = 0x7927;
            }
            else if(data[13] == '\x79' && data[14] == '\x28'){
                errStr = tr("舱门电机复位失败！");
                errCode = 0x7928;
            }
            else if(data[13] == '\x79' && data[14] == '\x29'){
                errStr = tr("舱门复位光耦异常，光耦应处在非档光状态！");
                errCode = 0x7929;
            }
            else if(data[13] == '\x79' && data[14] == '\x2a'){
                errStr = tr("舱门电机运动超时！");
                errCode = 0x792a;
            }
            else if(data[13] == '\x79' && data[14] == '\x2b'){
                errStr = tr("未出舱门！");
                errCode = 0x792b;
            }
            else if(data[13] == '\x79' && data[14] == '\x2c'){
                errStr = tr("舱门工作光耦异常，光耦应处在档光状态！");
                errCode = 0x792c;
            }
            else if (data[13] == '\x79' && data[14] == '\x2d'){
                errStr = tr("调焦电机复位失败！");
                errCode = 0x792d;
            }
            else if (data[13] == '\x79' && data[14] == '\x2e'){
                errStr = tr("调焦光耦异常！");
                errCode = 0x792e;
            }
            else if (data[13] == '\x79' && data[14] == '\x2f'){
                errStr = tr("调焦电机运动超时！");
                errCode = 0x792f;
            }
            else if(data[13] == '\x79' && data[14] == '\x30'){
                errStr = tr("申请关闭温控失败！");
                errCode = 0x7930;
            }
            else if(data[13] == '\x79' && data[14] == '\x31'){
                errStr = tr("定时时间超时！");
                errCode = 0x7931;
            }
            else if(data[13] == '\x79' && data[14] == '\x32'){
                errStr = tr("舱门工作光耦异常，光耦应处在非档光状态！");
                errCode = 0x7932;
            }
            else if (data[13] == '\x79' && data[14] == '\x33'){
                errStr = tr("舱门电机运行到出舱位，出舱光耦状态不符合预期（应档光状态）！");
                errCode = 0x7933;
            }
            else if (data[13] == '\x79' && data[14] == '\x34'){
                errStr = tr("出舱光耦状态不符合预期，光耦应处在非档光状态！");
                errCode = 0x7934;
            }
            else if (data[13] == '\x79' && data[14] == '\x35'){
                errStr = tr("舱门复位光耦异常，光耦应处在档光状态！");
                errCode = 0x7935;
            }
        }
        emit errOccur(tr("故障码：")+QString::number(errCode,16)+"\n"+errStr);
        Log::Logdb(LOGTYPE_ERR,errCode,errStr);
        lampDo(LampState::Lamp_Light_orange);
        qDebug()<<"Lamp3";
        sequenceCancel();
    }
}
//因为打印动作可能同步执行，所有独立接收函数
void Sequence::PrintFinish(QByteArray data){
    if (data[7] == '\xB1'){
        if (data[8] == '\x00')
            emit sequenceFinish(SequenceResult::Result_Print_finish);
        else
            emit sequenceFinish(SequenceResult::Result_Print_Error);
    }
}

static int useTestId;
static int TestFinish(){
    qDebug()<<"TestFinish";
    qDebug()<<"panelCode="<<ExGlobal::panelCode();
    if (ExGlobal::panelCode() == ExGlobal::DemoPanelCode)
        DataHandler::HandleFileData(QCoreApplication::applicationDirPath()+"/RawData.csv",Log::getDir());
    else if (ExGlobal::panelCode().startsWith("3"))
        DataHandler::SaveOnePointData(useTestId,Log::getDir());
    else
        DataHandler::SaveData(useTestId,Log::getDir());
    ExGlobal::pTestModel->AddTest(useTestId);
    ExGlobal::pTestResultModel->setTestid(useTestId,ExGlobal::panelCode());
    sequence->printPDFResult();    
    return 0;
}

void Sequence::TestDataHandleFinish(int result){
    if (result == 0)
    {
        ExGlobal::getPtr()->setSampleCode("");
        ExGlobal::getPtr()->setSampleInfo("");
        ExGlobal::getPtr()->setSampleRemark("");
        currSequenceId = SequenceId::Sequence_Idle;
        Log::setDir(QCoreApplication::applicationDirPath());
        emit sequenceFinish(SequenceResult::Result_Test_finish);
    }
}

void Sequence::FinishSequence()
{
    bool doorChanged = false;
    SequenceResult out = SequenceResult::Result_NULL;
    qDebug()<<"FinishSequence:"<<currSequenceId;

    if (currSequenceId == SequenceId::Sequence_SelfCheck)
    {
        out = SequenceResult::Result_SelfCheck_ok;
    }
    else if(currSequenceId == SequenceId::Sequence_CloseBox)
    {
        if (bDoorState){
            bDoorState = false;
            bDoorState2 = false;
            doorChanged = true;
        }
        out = SequenceResult::Result_CloseBox_ok;
    }
    else if(currSequenceId == SequenceId::Sequence_OpenBox || currSequenceId == SequenceId::Sequence_ErrOpenBox)
    {
        if (!bDoorState){
            bDoorState = true;
            bDoorState2 = true;
            doorChanged = true;
        }
        out = SequenceResult::Result_OpenBox_ok;
    }
    else if(currSequenceId == SequenceId::Sequence_DoorToWork)
    {
        out = SequenceResult::Result_DoorToWork_ok;
    }
    else if(currSequenceId == SequenceId::Sequence_DoorFromWork)
    {
        out = SequenceResult::Result_DoorToWork_ok;
    }
    else if(currSequenceId == SequenceId::Sequence_CannelTest)
    {
        camera->closeCamera();
        out = SequenceResult::Result_CannelTest_ok;
        testSecondTime->stop();
        Log::setDir(QCoreApplication::applicationDirPath());
    }
    else if(currSequenceId == SequenceId::Sequence_Test)
    {
        camera->closeCamera();
        currSequenceId = SequenceId::Sequence_Idle;
        qDebug()<<"Itemsize"<<imageAna->getItem().size();//imageCapture->openCamera();
        qDebug()<<"panelCode"<<ExGlobal::panelCode()<<ExGlobal::DemoPanelCode;
        if (imageAna->getItem().size() > 20 || ExGlobal::panelCode() == ExGlobal::DemoPanelCode
                || ExGlobal::panelCode().startsWith("3"))
        {
            useTestId = testMgr->TestClose(2);
            ExGlobal::MethodToThread(TestFinish,this,SLOT(TestDataHandleFinish(int)));
            out = SequenceResult::Result_Test_HandleData;
        }
        else
        {
            testMgr->TestClose(1);
            out = SequenceResult::Result_Test_unfinish;
            Log::setDir(QCoreApplication::applicationDirPath());
        }
        testSecondTime->stop();        
    }
    else if(currSequenceId == SequenceId::Sequence_LoopTest)
    {
        if (continueLoopTest())
            return;
        else
        {
            //imageCapture->closeCamera();
            camera->closeCamera();
            currSequenceId = SequenceId::Sequence_Idle;
            testMgr->TestClose(3);
            out = SequenceResult::Result_LoopTest_finish;
        }
    }
    currSequenceId = SequenceId::Sequence_Idle;
    if (out == SequenceResult::Result_Test_HandleData)
    {
        currSequenceId = SequenceId::Sequence_Test;
        //UIHandler::sendUIMsg("main/2");
        UIHandler::GoPage(UIHandler::PageId::Page_Main);
    }

    lampDo(LampState::Lamp_Light_blue);
    qDebug()<<"Lamp4";
    if (out != SequenceResult::Result_NULL)
        emit sequenceFinish(out);
    if (doorChanged)
        emit doorStateChanged();
}

bool Sequence::ReadTestProcess(QString panel)
{
    QFile xmlfile(panel+".xml");
    qDebug("ReadTestProcess");
    if (!xmlfile.open(QFile::ReadOnly))
    {
        errReceive(ERROR_CODE_FILE_READ);
        qDebug()<<"file err:"<<xmlfile.error()<<",string:"<<xmlfile.errorString()<<",file:"<<xmlfile.fileName()<<",dir:"<<QDir::currentPath()<<",apppath:"<<QCoreApplication::applicationDirPath();
        return false;
    }

    if (!doc.setContent(&xmlfile))
    {
        xmlfile.close();
        qDebug()<<"xml setContent err return";
        errReceive(ERROR_CODE_FILE_FORMAT);
        return  false;
    }
    xmlfile.close();

    QDomElement root = doc.documentElement();
    if (root.hasAttribute("ProjectMode")){
        //ExGlobal::ProjectMode = root.attribute("ProjectMode").toInt();
    }

    if (root.hasAttribute("DoorOut")){
        ExGlobal::DoorOut = root.attribute("DoorOut").toInt();
    }

    if (root.hasAttribute("DefaultPanelCode"))
    {
        for (QDomElement panel = root.firstChildElement("Panel"); !panel.isNull(); panel = panel.nextSiblingElement("Panel"))
        {
            if (panel.attribute("Panel_ID")==root.attribute("DefaultPanelCode"))
            {
                qDebug()<<"DefaultPanelCode:"<<panel.attribute("Panel_ID");
                ExGlobal::setPanelName(panel.attribute("Panel_c"));
                ExGlobal::setPanelId(panel.attribute("Panel_ID"));
                ExGlobal::setPanelCode(panel.attribute("Test_ID"));
                break;
            }
        }
    }

    return true;
}

bool Sequence::ReadMask(QString mask)
{
    qDebug()<<"ReadMask:"<<mask;
    QFile maskfile(mask+".bin");
    if (!maskfile.open(QFile::ReadOnly))
    {
        errReceive(ERROR_CODE_FILE_MODEL);
        qDebug()<<"file err:"<<maskfile.error()<<",string:"<<maskfile.errorString()<<",file:"<<maskfile.fileName()<<",dir:"<<QDir::currentPath()<<",apppath:"<<QCoreApplication::applicationDirPath();
        return false;
    }
    QDataStream in(&maskfile);
    char r[121];
    int readlen = in.readRawData(r,121);
    qDebug()<<"ReadMask:readlen="<<readlen;
    imageAna->SetMask(r,0);
    maskfile.close();
    return true;
}

bool Sequence::WriteTestProcess(QString panel)
{
    QFile xmlfile(panel+".xml");

    if (!xmlfile.open(QFile::WriteOnly|QFile::Truncate))
        return false;
    QTextStream out(&xmlfile);
    doc.save(out,4);
    xmlfile.close();
    return true;
}

bool Sequence::DoAction(QDomElement action,bool isChild)
{
    nDuration = action.attribute("Duration").toInt();
    nPreTime = action.attribute("pretime").toInt();
    qDebug()<<"DoAction:"<<action.attribute("Device")<<"step:"<<action.attribute("No")<<"duration:"<<action.attribute("Duration")<<"Value:"<<action.attribute("ActionValue")<<"Param:"<<action.attribute("ActionParam1");

    message.sprintf("No: %d; Duration:%d; Device:",action.attribute("No").toInt(),action.attribute("Duration").toInt());
    message += action.attribute("Device")+","+action.attribute("ActionValue")+","+action.attribute("ActionParam1")+","+action.attribute("ActionParam2");
    Log::LogWithTime(message);

    if (isLoopTesting())
        //Log::LogByFile("LoopTest.txt",QString("第%1/%2大循环,%3").arg(loopTestCurrCount).arg(loopTestCount).arg(message));
        Log::LogByFile("LoopTest.txt",message);

    if (isChild){
        QDomElement p = action.parentNode().toElement();
        int cycle = p.attribute("currcycle").toInt();
        if (action.attribute("No").toInt() == 1){
            Log::LogTime(QString("\ncycle=%1").arg(cycle));
        }
    }
    Log::LogTime(message);
    bFinishAction = false;

    if (action.attribute("Device")=="CameraCapture")
    {
        currCameraCaptureType = action.attribute("ActionValue").toInt();
        currCameraCycle = 0;
        QString captureFileName = "";
        if(isChild){
            QDomElement p = action.parentNode().toElement();
            int cycle = p.attribute("currcycle").toInt();
            currCameraCycle = cycle;
            /*
            if (action.attribute("ActionValue") == "4")
                captureFileName = "cycleb"+QString("%1").arg(cycle,2,10,QChar('0'));
            else
                captureFileName = "cycle"+QString("%1").arg(cycle,2,10,QChar('0'));
                //*/
        }
        else if (action.attribute("ActionValue") == "1")
        {
            captureFileName = "Dry";
        }
        else if (action.attribute("ActionValue") == "2")
        {
            captureFileName = "Black";
        }
        else if (action.attribute("ActionValue") == "3")
            captureFileName = "Fill";

        currOrder = '\xA0';
        /*
        imageCapture->setFileName(captureFileName);
        imageCapture->setImageCount(action.attribute("ActionParam1").toInt());
        if (imageCapture->waitStop())
            imageCapture->start();
            //*/
        camera->capture(captureFileName,action.attribute("ActionParam1").toInt());
    }
    else if(action.attribute("Device")=="Door" || action.attribute("Device")=="Light" || action.attribute("Device")=="Temperature"
            ||action.attribute("Device")=="V1" || action.attribute("Device")=="V2" || action.attribute("Device")=="V3" || action.attribute("Device")=="V123"
            ||action.attribute("Device")=="VP" || action.attribute("Device")=="Pump" || action.attribute("Device")=="Query"
            ||action.attribute("Device")=="SetPID" || action.attribute("Device")=="Fan")
    {
        QByteArray send = ActionParser::ActionToByte(action);
        currOrder = send[7];
        serialMgr->serialWrite(send);
    }
    else if(action.attribute("Device")=="Other")
    {
        if (action.attribute("ActionValue") == "1"){    //曝光时间设置
            bFinishAction = true;
            camera->setabsExpose(action.attribute("ActionParam1").toInt());
        }
        else if(action.attribute("ActionValue") == "2"){    //pause
            bFinishAction = true;
            nDuration = 0;
            testSecondTime->stop();
            emit sequenceFinish(SequenceResult::Result_Pause);
            return false;
        }
    }
    else if(action.attribute("Device")=="Test")
    {
       if(action.attribute("ActionValue") == "1"){    //pause
            bFinishAction = true;
            nDuration = 0;
            testSecondTime->stop();
            emit sequenceFinish(SequenceResult::Result_Pause);
            return false;
        }
    }

    if (isChild){
        QDomElement p = action.parentNode().toElement();
        nPreTime += p.attribute("Duration").toInt()*(p.attribute("currcycle").toInt()-1) + p.attribute("pretime").toInt();
        message.sprintf("总时间：%d秒;\t完成时间：%d秒;\t第 %d 步; \t耗时：%d秒\n\t第%d循环; \t第 %d 步; \t耗时：%d秒\t设备：",sequenceAction.attribute("Duration").toInt()/1000,nPreTime/1000,
                        p.attribute("No").toInt(),p.attribute("Duration").toInt()/1000,p.attribute("currcycle").toInt(),action.attribute("No").toInt(),action.attribute("Duration").toInt()/1000);
    }
    else
        message.sprintf("总时间：%d秒;\t完成时间：%d秒;\t第 %d 步; \t耗时：%d秒\t设备：",sequenceAction.attribute("Duration").toInt()/1000,nPreTime/1000,action.attribute("No").toInt(),action.attribute("Duration").toInt()/1000);
    message += action.attribute("Device")+","+action.attribute("ActionValue")+","+action.attribute("ActionParam1")+","+action.attribute("ActionParam2");

    if (isTesting()||isLoopTesting()){
        emit processFinish(sequenceAction.attribute("Duration").toInt(),nPreTime);
        int remain = (sequenceAction.attribute("Duration").toInt() - nPreTime)/1000;
        QString title = tr("正在测试，预计剩余")+QString::number(remain)+tr("秒");
        if (remain > 60)
            title = tr("正在测试，预计剩余")+QString::number((remain/60)+1)+tr("分钟");
        if (isLoopTesting()){
            int oneloop = sequenceAction.attribute("Duration").toInt()/1000;
            remain += (loopTestCount-loopTestCurrCount)*oneloop;
            title = tr("耐力测试，正在测试第")+QString::number(loopTestCurrCount)+"/"+QString::number(loopTestCount)+tr("循环，预计剩余")+QString::number(remain)+tr("秒");
            nPreTime = nPreTime + (loopTestCurrCount-1)*oneloop*1000;
            emit titleNotify(nPreTime/(oneloop*loopTestCount)+100,title);
            Log::LogByFile("LoopTest.txt",QString("total time=%1,completion time=%2").arg(oneloop*loopTestCount).arg(nPreTime));
        }
        else
        {
            //emit titleNotify(nPreTime/(sequenceAction.attribute("Duration").toInt()/1000)+100,title);
            Log::LogWithTime(QString("total time=%1,completion time=%2").arg(sequenceAction.attribute("Duration")).arg(nPreTime));
        }
    }

    Log::LogTime(QString("start timer,duration=%1,type=%2").arg(nDuration).arg(action.attribute("DurationType")));
    durationState = TimeState::running;
    if (nDuration == 0)
    {
        SequenceTimeout();
    }
    else if(action.attribute("DurationType")=="1")
    {
        Log::LogTime(QString("Timing before action"));
        timer->start(nDuration);
    }
    else
    {
        Log::LogTime(QString("Timing after action"));
        durationState = TimeState::wait;
    }

    return true;
}

QString Sequence::sequenceMessage(){
    return message;
}
bool Sequence::FindAction(bool bFinishAction)
{
    int totalStep = sequenceAction.attribute("steps").toInt();
    int currStep = sequenceAction.attribute("currstep").toInt();
    //int nTotalTime = sequenceAction.attribute("Duration").toInt();

    QDomElement e = sequenceAction.firstChildElement();
    while (!e.isNull()) {
        if (e.hasAttribute("No") && e.attribute("No").toInt() == currStep){
            if (e.tagName() == "Action"){
                if (!bFinishAction)
                    return DoAction(e,false);
                else if(currStep == totalStep)
                {
                    //emit processFinish(nTotalTime,nTotalTime);
                    FinishSequence();
                    return true;
                }
                else
                {
                    currStep++;
                    sequenceAction.setAttribute("currstep",currStep);
                    return FindAction(false);
                }
            }else if(e.tagName() == "Actions"){
                int nCycle = e.attribute("cycle").toInt();
                int nCurrCycle = e.attribute("currcycle").toInt();
                int nChildCurrStep = e.attribute("currstep").toInt();
                int nChildTotalStep = e.attribute("steps").toInt();
                QDomNodeList list = e.childNodes();
                for (int i = 0; i<list.count();i++)
                {
                    QDomNode n = list.at(i);
                    if (n.isElement()){
                        QDomElement ne = n.toElement();
                        if (ne.tagName() == "Action" && ne.attribute("No").toInt() == nChildCurrStep){
                            if (!bFinishAction)
                                return DoAction(ne,true);
                            else if(nChildCurrStep == nChildTotalStep){
                                if (nCurrCycle == nCycle){
                                    if (currStep == totalStep){
                                        //emit processFinish(nTotalTime,nTotalTime);
                                        FinishSequence();
                                        return true;
                                    }
                                    else{
                                        currStep++;
                                        sequenceAction.setAttribute("currstep",currStep);
                                    }
                                }
                                else {
                                    nChildCurrStep = 1;
                                    nCurrCycle++;
                                    e.setAttribute("currstep",nChildCurrStep);
                                    e.setAttribute("currcycle",nCurrCycle);
                                }
                            }
                            else
                            {
                                nChildCurrStep++;
                                e.setAttribute("currstep",nChildCurrStep);
                            }
                            return FindAction(false);
                        }
                    }
                }
            }
        }
        e = e.nextSiblingElement();
    }

    return true;
}

int Sequence::CalSteps(QDomElement element)
{
    int nStep = 0;
    QDomNode node = element.firstChild();
    while(!node.isNull())
    {
        QDomElement e = node.toElement();
        if (!e.isNull() && (e.tagName()=="Action"||e.tagName()=="Actions"))
            nStep++;
        node = node.nextSibling();
    }
    return nStep;
}

bool Sequence::FormatAction(){
    bool result = false;
    int totalStep = CalSteps(sequenceAction);
    int nFinishTime = 0;

    sequenceAction.setAttribute("currstep",1);
    sequenceAction.setAttribute("steps",totalStep);

    for (int i = 1; i <= totalStep; i++){
        QDomNode node = sequenceAction.firstChild();
        while (!node.isNull()) {
            QDomElement e = node.toElement();
            if (!e.isNull()&& e.hasAttribute("No") && e.attribute("No").toInt() == i){
                e.setAttribute("pretime",nFinishTime);
                if (e.tagName()=="Action")
                {
                    nFinishTime += e.attribute("Duration").toInt();
                    if (e.hasAttribute("ActionTime"))
                        nFinishTime += e.attribute("ActionTime").toInt();
                }
                else if(e.tagName()=="Actions"){
                    int childTotalStep = CalSteps(e);
                    e.setAttribute("steps",childTotalStep);
                    int cycle = e.attribute("cycle").toInt();
                    int nChildFinshTime = 0;
                    for (int j = 1; j <= childTotalStep; j++)
                    {
                        QDomNode cnode = e.firstChild();
                        bool cresult = false;
                        e.setAttribute("currcycle",1);
                        e.setAttribute("currstep",1);
                        while(!cnode.isNull())
                        {
                            QDomElement ce = cnode.toElement();
                            if (!ce.isNull()&& ce.hasAttribute("No") && ce.attribute("No").toInt() == j){
                                ce.setAttribute("pretime",nChildFinshTime);
                                nChildFinshTime += ce.attribute("Duration").toInt();
                                if (ce.hasAttribute("ActionTime"))
                                    nChildFinshTime += ce.attribute("ActionTime").toInt();
                                cresult = true;
                                break;
                            }
                            cnode = cnode.nextSibling();
                        }
                        if (!cresult)
                            return false;
                    }
                    e.setAttribute("Duration",nChildFinshTime);
                    nFinishTime += cycle*nChildFinshTime;
                }
                result = true;
                break;
            }
            node = node.nextSibling();
        }
        if (!result)
            return false;
    }
    sequenceAction.setAttribute("Duration",nFinishTime);
    return true;
}

static bool FirstCheckSensor = true;
void Sequence::setSenorState(char char1, char char2)
{
    bool sensor;
    //qDebug()<<"setSenorState,char1:"<<char1<<",char2:"<<char2;
    qDebug()<<"setSensorState,bBoxState:"<<bBoxState<<",bDoorState:"<<bDoorState<<",char1:"<<QString::number(char1,16)<<",char2:"<<QString::number(char2,16);
    if (char1&0x08)
        sensor = true;
    else
        sensor = false;
    if (FirstCheckSensor == true || sensor != bBoxState)
    {
        bBoxState = sensor;
        emit boxStateChanged();
    }

    if (char1&0x02)
        sensor = false;
    else
        sensor = true;
    if (sensor != bDoorState)
    {
        bDoorState = sensor;
        emit doorStateChanged();
    }

//opendoor sensor
    if (char2&0x80)
        bDoorState2 = true;
    else
        bDoorState2 = false;

    FirstCheckSensor = false;
    qDebug()<<"bBoxState:"<<bBoxState<<"bDoorState:"<<bDoorState;
}

double Sequence::getDefinition(){
    if (ExGlobal::ClearMode == 1)
        return imageAna->GetDefinition2(camera->getyData(),camera->getImageType());
    if (ExGlobal::ClearMode == 2)
        return imageAna->GetDefinition3(camera->getyData(),camera->getImageType());
    if (ExGlobal::ClearMode == 3)
        return imageAna->GetMeanLight(camera->getyData(),camera->getImageType());
    if (ExGlobal::ClearMode == 4)
        return imageAna->GetCircularSize(camera->getyData(),camera->getImageType());
    return imageAna->GetDefinition(camera->getyData(),camera->getImageType());
}

static double autoFocus_FirstClarityValue;
void Sequence::CameraView(QImage img)
{
    //imageProvider->img = img;
    //emit callQmlRefeshView();
    emit callRefeshView(img);
    if (bAutoFocus){
        Log::LogByFile("Focus.txt",QString("CameraView,JumpStep:%1,CurrPoint:%2,ClearValue:%3,ClearPoint:%4").arg(autoFocus_JumpStep).arg(autoFocus_CurrPoint).arg(autoFocus_ClarityValue).arg(autoFocus_ClarityPoint));
        qDebug()<<"JumpStep:"<<autoFocus_JumpStep<<"CurrPoint:"<<autoFocus_CurrPoint<<"ClarityValue:"<<autoFocus_ClarityValue<<"ClarityPoint:"<<autoFocus_ClarityPoint;
        if (autoFocus_JumpStep == 2){
            if (autoFocus_CurrPoint > 0)
            {
                double value = getDefinition();
                qDebug()<<"currValue:"<<value;
                if (value > autoFocus_ClarityValue)
                {
                    autoFocus_ClarityValue = value;
                    autoFocus_ClarityPoint = autoFocus_CurrPoint;
                }

                if (autoFocus_CurrPoint - autoFocus_ClarityPoint > AUTOFOCUS_MAXOFFSET || autoFocus_CurrPoint > (AUTOFOCUS_MAX-AUTOFOCUS_MIN)/AUTOFOCUS_STEP){
                    if (autoFocus_dec == false && autoFocus_ClarityPoint == 1){
                        autoFocus_dec = true;
                        autoFocus_CurrPoint = 0;
                        autoFocus_FirstClarityValue = autoFocus_ClarityValue;
                        autoFocus_ClarityValue = 0;
                    }
                    else{
                        int focusResult;
                        if (autoFocus_dec == true){
                            if (autoFocus_ClarityValue < autoFocus_FirstClarityValue)
                                focusResult = 0;
                            else
                                focusResult = AUTOFOCUS_MAX-AUTOFOCUS_STEP*(autoFocus_ClarityPoint-1)-AUTOFOCUS_MIN;
                        }
                        else {
                            focusResult = AUTOFOCUS_STEP*(autoFocus_ClarityPoint-1);
                        }
                        dirctAction("Focus",2,focusResult+AUTOFOCUS_MIN,0,0);
                        emit autoFocusNotify(1,focusResult);
                        ExGlobal::updateCaliParam("CamFocus",focusResult);

                        bAutoFocus = false;
                        bFocused = true;
                        //imageCapture->stopCamera();
                        //imageCapture->captureMode = ImageCapture::CaptureMode::Capture;
                        if (isTesting())
                        {
                            camera->stopCamera();
                            dirctAction("Light",1,0,0,0);
                        }
                        //else
                        //    dirctAction("Light",5,0,0,0);
                        Log::LogByFile("Focus.txt",QString("CameraView,focus result:%1,AUTOFOCUS_MIN=%2").arg(focusResult).arg(AUTOFOCUS_MIN));
                        return;
                    }
                }
            }

            autoFocus_CurrPoint++;
            autoFocus_JumpStep = 0;
            if (autoFocus_dec == true){
                dirctAction("Focus",2,AUTOFOCUS_MAX-AUTOFOCUS_STEP*(autoFocus_CurrPoint-1),0,0);
                emit autoFocusNotify(0,AUTOFOCUS_MAX-AUTOFOCUS_STEP*(autoFocus_CurrPoint-1)-AUTOFOCUS_MIN);
            }
            else{
                dirctAction("Focus",2,AUTOFOCUS_STEP*(autoFocus_CurrPoint-1)+AUTOFOCUS_MIN,0,0);
                emit autoFocusNotify(0,AUTOFOCUS_STEP*(autoFocus_CurrPoint-1));
            }
        }
        else if(autoFocus_JumpStep == 1)
            autoFocus_JumpStep = 2;
    }
    else if (bCaptureLight){
        qDebug()<<"bCaptureLight"<<autoFocus_JumpStep;
        if (++autoFocus_JumpStep>1){
            autoFocus_JumpStep = 0;
            camera->stopCamera();
            dirctAction("Light",1,0,0,0);
            bCaptureLight = false;
            emit refreshLightValue();
        }
    }
}

bool Sequence::startView(int id){
    if (id == 0){
        /*
        if (imageCapture->waitStop())
        {
            actionDo("Light",2,0,0,0);
            imageCapture->imagetype = 0;
            imageCapture->openCamera();
            imageCapture->captureMode = ImageCapture::CaptureMode::View;
            imageCapture->setImageCount(99999);
            imageCapture->start();
        }
        //*/
        actionDo("Light",5,0,0,0);
        camera->preview(id);
    }
    return true;
}

bool Sequence::stopView(){
    //imageCapture->closeCamera();
    camera->closeCamera();
    return true;
}

bool Sequence::saveView(){
    QDir dir(QCoreApplication::applicationDirPath());
    QStringList filter;
    filter<<QString("tmp*.bmp");
    dir.setNameFilters(filter);
    QFileInfoList fileList = dir.entryInfoList();
    char filenames[100] = {0};
    sprintf(filenames,"%s/tmp%d.bmp",QCoreApplication::applicationDirPath().toLatin1().data(),fileList.count()+1);
    qDebug()<<"saveView"<<filenames<<"\n\n\n";
    //imageProvider->img.save(filenames);
    sprintf(filenames,"%s/tmp%d.raw",QCoreApplication::applicationDirPath().toLatin1().data(),fileList.count()+1);
    camera->saveRaw(filenames);
    return true;
}

void Sequence::errReceive(ERROR_CODE code){
    QString errStr;
    if (code == ERROR_CODE_TEMP_CONNECT){
        errStr = tr("温控板通讯出错！");
    }
    else if(code == ERROR_CODE_CTRL_CONNECT)
        errStr = tr("驱动板通讯出错！");
    else if(code >= ERROR_CODE_CAM_CONNECT && code < ERROR_CODE_FILE_READ)
        errStr = tr("摄像头通讯出错！");
    else if(code == ERROR_CODE_FILE_READ)
        errStr = tr("配置文件读取出错！");
    else if(code == ERROR_CODE_FILE_FORMAT)
        errStr = tr("配置文件格式出错！");
    else if(code == ERROR_CODE_FILE_MODEL)
        errStr = tr("模板文件读取出错！");
    else if(code == ERROR_CODE_DISK_FULL)
        errStr = tr("磁盘已满！");
    emit errOccur(tr("故障码：")+QString::number(code,16)+"\n"+errStr);
    lampDo(LampState::Lamp_Light_orange);
    qDebug()<<"Lamp5";
}

int Sequence::getAbs(){
    //return imageCapture->getabsExpose();
    return camera->getabsExpose();
}

bool Sequence::setAbs(int value){
    //return imageCapture->setabsExpose(value);
    return camera->setabsExpose(value);
}

int Sequence::getGain(){
    //return imageCapture->getGain();
    return camera->getGain();
}

bool Sequence::setGain(int value){
    //return imageCapture->setGain(value);
    return camera->setGain(value);
}

int Sequence::getWhiteBalance(){
    //return imageCapture->getWhite();
    return camera->getWhite();
}

bool Sequence::setWhiteBalance(int value){
    //return imageCapture->setWhite(value);
    return camera->setWhite(value);
}

void test(const char *str){
    qDebug()<<QString::number(str[0],16)<<QString::number(str[1],16)<<QString::number(str[2],16)<<QString::number(str[3],16)<<QString::number(str[4],16)<<QString::number(str[5],16);
}

void Sequence::lxDebug(){
    qDebug()<<"lxDebug";
    //qr->DebugQr();
    //UIHandler::sendUIMsg("00/main/1");
}

void Sequence::showAnaImg(int type, int light){
    //imageProvider->anaMainImg = imageAna->getMainImg(type,light);
    emit callQmlRefeshAnaMainImg();
}

bool Sequence::listNextAction(bool first){
    qDebug()<<"listNextAction:count="<<actList.count();
    if (!first && actList.count()>0)
        actList.removeFirst();

    if (actList.count()>0){
        action act = actList.first();
        if (act.device == "Qrcode"){
            currOrder = '\xB0';
            qr->Qr();
        }
        else if(act.device == "Pierce"){
            currOrder = '\xB1';
            qr->Pierce();
        }
        else if(act.device == "SwitchDoor"){
/*
 * 前面已经执行查询sensor状态，合仓：1，Pump复位，2，Pump下移25000步，3，舱门进，4：Pump到工装位置
 */
            qDebug()<<"SwitchDoor"<<ActionParser::pumpBottom();
            if (bDoorState)
            {
                if (!ActionParser::pumpBottom()){
                    act.device = "Pump";
                    act.value = 8;
                    actList.append(act);

                    if (hasTime0119){
                        act.device = "Door";
                        act.value = 12;
                        act.param1 = 12;
                        act.param2 = 5;
                    }else{
                        act.device = "Door";
                        act.value = 5;
                    }
                    actList.append(act);

                    act.device = "Pump";
                    act.value = 3;
                    actList.append(act);

                    //query box state after close door
                    /*
                    act.device = "Query";
                    act.value = 3;
                    actList.append(act);
                    */
                    act.device = "Pump";
                    act.value = 1;
                }
                else{
                    act.device = "Pump";
                    act.value = 3;
                    actList.append(act);

                    //query box state after close door
                    /*
                    act.device = "Query";
                    act.value = 3;
                    actList.append(act);
*/
                    if (hasTime0119){
                        act.device = "Door";
                        act.value = 12;
                        act.param1 = 12;
                        act.param2 = 5;
                    }else{
                        act.device = "Door";
                        act.value = 5;
                    }
                }
            }
            else {
                if (!ActionParser::pumpBottom()){
                    act.device = "Pump";
                    act.value = 1;
                    actList.append(act);

                    act.device = "Pump";
                    act.value = 8;
                    actList.append(act);
                }
                act.device = "Door";
                if (hasTime0119){
                    act.value = 11;
                    act.param2 = 5;
                    act.param3 = 12;
                }
                else
                    act.value = 1;
                act.param1 = ExGlobal::DoorOut;
            }
            QByteArray send = ActionParser::ParamToByte(act.device,act.value,act.param1,act.param2,act.param3);
            currOrder = send[7];
            serialMgr->serialWrite(send);
        }
        else if(act.device == "ErrOpenDoor"){
            if (!bDoorState2){
                ErrOpenDoorCount += 500;
                if (ErrOpenDoorCount > ERROPENDOOR_MAXLENGTH)
                    return false;

                act.device = "Query";
                act.value = 3;
                actList.append(act);

                act.device = "ErrOpenDoor";
                actList.append(act);

                act.device = "Door";
                act.value = 3;
                act.param1 = 500;

                QByteArray send = ActionParser::ParamToByte(act.device,act.value,act.param1,act.param2,act.param3);
                currOrder = send[7];
                serialMgr->serialWrite(send);
            }
            else
                return false;
        }
        else {
            QByteArray send = ActionParser::ParamToByte(act.device,act.value,act.param1,act.param2,act.param3);
            currOrder = send[7];
            serialMgr->serialWrite(send);
        }
        return true;
    }

    return false;
}

void Sequence::qrDect(){
    actList.clear();
    action act;

    if (bQrOpenLight)
    {
        act.device = "Led";
        act.value = 62;
        actList.append(act);
    }

    act.device = "Qrcode";
    actList.append(act);

    if (bQrOpenLight)
    {
        act.device = "Led";
        act.value = 63;
        actList.append(act);
    }

    listNextAction(true);
}

void Sequence::qrSet(bool bopenlight, bool scale, bool handlimage, int bin, int pox){
    bQrOpenLight = bopenlight;
    qr->binValue = bin;
    qr->poxValue = pox;
    qr->handleimage = handlimage;
    qr->scale = scale;
}

void Sequence::PierceDect(){
    actList.clear();
    action act;

    act.device = "Led";
    act.value = 0x3e;
    actList.append(act);

    act.device = "Pierce";
    actList.append(act);

    act.device = "Led";
    act.value = 0x3f;
    actList.append(act);

    listNextAction(true);
}

int Sequence::decodeQr(QString strQr){
    qDebug()<<"decodeQr"<<strQr;
    QStringList record = strQr.split(' ');
    if (record.size() < 6) return 2;
    if (!record[0].startsWith("FLASHDX") && !record[0].startsWith("FlashDx")) return 2;
    record[0].remove(0,7);
    sequenceSetPanel(record[1]);
    if (record[0] == "1"){
        ExGlobal::setReagentBox(record[2]);
        ExGlobal::setBoxSerial(QString("Lot# %1").arg(record[3]));
        QDateTime time = QDateTime::fromString(record[4],"yyyyMMddhh");
        ExGlobal::validDateTime = time.addDays(record[5].toInt());
        qDebug()<<"validDate"<<ExGlobal::validDateTime.toString("yyyyMMdd-hh:mm:ss");
        if (time.addDays(record[5].toInt()).toTime_t() < QDateTime::currentDateTime().toTime_t())
            return 1;
        return 0;
    }
    else if (record[0] == "11"){
        ExGlobal::setBoxSerial(QString("Lot# %1").arg(record[2]));
        record[3] = "20"+record[3];
        QDateTime time = QDateTime::fromString(record[3],"yyyyMMdd");
        ExGlobal::validDateTime = time.addDays(record[4].toInt());
        qDebug()<<"validDate"<<ExGlobal::validDateTime.toString("yyyyMMdd-hh:mm:ss")<<ExGlobal::reagentBox();
        if (time.addDays(record[4].toInt()).toTime_t() < QDateTime::currentDateTime().toTime_t())
            return 1;
        return 0;
    }
    return 2;
}

void Sequence::SwitchDoor(bool error){
    actList.clear();
    action act;

    act.device = "Query";
    act.value = 3;
    actList.append(act);

    if (error)
    {
        act.device = "ErrOpenDoor";
        ErrOpenDoorCount = 0;
    }
    else
        act.device = "SwitchDoor";
    actList.append(act);

    listNextAction(true);
}

void Sequence::DoorToWork(bool toWork){
    actList.clear();
    action act;

    act.device = "Door";
    act.value = 5;
    actList.append(act);

    if (toWork){
        act.device = "Door";
        act.value = 7;
        actList.append(act);
    }

    listNextAction(true);
}
void Sequence::fan1SetSpeed(int speed)
{
    qDebug()<<"fan1SetSpeed:"<<speed<<"prespeed:"<<t_fan1Speed;
    t_fan1Speed = speed;
    emit fan1SpeedChanged();
}

void Sequence::autoFocus(){
    Log::LogByFile("Focus.txt",QString("bAutoFocus:%1").arg(bAutoFocus));
    if (bAutoFocus == false){
        /*
        if (!imageCapture->isRunning()){
            dirctAction("Light",2,0,0,0);
            imageCapture->imagetype = 0;
            if (imageCapture->captureMode == ImageCapture::CaptureMode::Idle)
                imageCapture->openCamera();
            else
                imageCapture->captureMode = ImageCapture::CaptureMode::View;
            imageCapture->setImageCount(99999);
            imageCapture->start();
        }
        //*/
        dirctAction("Light",2,0,0,0);
        camera->preview(1);

        dirctAction("Focus",1,0,0,0);
        bAutoFocus = true;
        autoFocus_CurrPoint = 0;
        autoFocus_ClarityValue = 0;
        autoFocus_JumpStep = 0;
        autoFocus_dec = false;
    }
}

void Sequence::startLight(){
    bCaptureLight = true;
    autoFocus_JumpStep = 0;
    dirctAction("Light",4,0,0,0);
    camera->preview(2);
}

bool Sequence::printTest(){    
    if (printer->isRunning())
        return false;
    printer->testTime = ExGlobal::pTestModel->getCurrTestDateTime();
    printer->validTime = ExGlobal::pTestModel->getCurrValidDateTime();
    printer->sampCode = ExGlobal::pTestModel->getCurrTestCode();
    printer->sampInfo = ExGlobal::pTestModel->getCurrTestInfo();
    printer->sampRemark = ExGlobal::pTestModel->getCurrTestRemark();
    printer->sampType = ExGlobal::getSampleTypeName(ExGlobal::pTestModel->getCurrTestType());
    printer->user = ExGlobal::pTestModel->getCurrTestUser();
    printer->checker = ExGlobal::pTestModel->getCurrTestChecker();
    //printer->panelName = ExGlobal::pTestModel->getCurrTestPanelName();
    //printer->refValid = ExGlobal::pTestResultModel->testValidCheck();
    printer->itemMap.clear();
    QList<int> item = ExGlobal::getBoxItemList();
    //int testID = ExGlobal::pTestResultModel->getTestid();
    for(int i = 0; i < item.length(); i++){        
        //printer->itemMap[ExGlobal::getPosFullName(item[i])] =
        //        ExGlobal::pTestResultModel->getItemResultByName(testID,ExGlobal::getPosName(item[i]));
    }
    printer->start();
    return true;
}

bool Sequence::printPDFResult(){
    printer->testTime = ExGlobal::pTestModel->getCurrTestDateTime();
    printer->validTime = ExGlobal::pTestModel->getCurrValidDateTime();
    printer->sampCode = ExGlobal::pTestModel->getCurrTestCode();
    printer->sampInfo = ExGlobal::pTestModel->getCurrTestInfo();
    printer->sampRemark = ExGlobal::pTestModel->getCurrTestRemark();
    printer->sampType = ExGlobal::getSampleTypeName(ExGlobal::pTestModel->getCurrTestType());
    printer->user = ExGlobal::pTestModel->getCurrTestUser();
    printer->checker = ExGlobal::pTestModel->getCurrTestChecker();
    //printer->panelName = ExGlobal::pTestModel->getCurrTestPanelName();
    //printer->refValid = ExGlobal::pTestResultModel->testValidCheck();
    printer->itemMap.clear();
    QList<int> item = ExGlobal::getBoxItemList();
    qDebug()<<"printPDFResult"<<item;
    int testID = ExGlobal::pTestResultModel->getTestid();
    for(int i = 0; i < item.length(); i++){
        //printer->itemMap[ExGlobal::getItemCode(item[i])] =
        //        ExGlobal::pTestResultModel->getItemResultByName(testID,ExGlobal::getPosName(item[i]));
    }
    printer->printResult(Log::getDir()+"/LinuxData");
    return true;
}

void Sequence::changeTitle(QString title){
    if (!isTesting() && !isLoopTesting())
        emit titleNotify(0, title);
}

void Sequence::setTitle(QString titleid){
    emit titleNotify(5,titleid);
}

void Sequence::hideTitle(bool hide){
    if (hide)
        emit titleNotify(1,"");
    else
        emit titleNotify(2,"");
}

bool Sequence::loopTest(QString testName, int count){
    qDebug()<<"Loop Test"<<loopTestName<<count;
    if (currSequenceId != SequenceId::Sequence_Idle)
        return false;
    currSequenceId = SequenceId::Sequence_LoopTest;
    QDomElement root = doc.documentElement();
    loopTestCount = count;
    loopTestCurrCount = 1;
    loopTestName = testName;
    for (sequenceAction = root.firstChildElement("Panel"); !sequenceAction.isNull(); sequenceAction = sequenceAction.nextSiblingElement("Panel"))
    {
        if (sequenceAction.attribute("Panel_c")==testName)
        {
            testStartTime = QDateTime::currentDateTime();
            QString current_time_str = testStartTime.toString("yyyyMMdd_hhmmss");
            Log::setDir(QCoreApplication::applicationDirPath()+"/"+current_time_str);
            //imageCapture->openCamera();
            camera->openCamera();
            ExGlobal::setPanelName(sequenceAction.attribute("Panel_c"));
            //ExGlobal::setReagentBox("202");
            imageAna->SetMask(ExGlobal::getReagentBox(ExGlobal::reagentBox()),0);

            if (!FormatAction())
                return false;
            testMgr->LoopTestCreate(sequenceAction.attribute("Panel_ID"),loopTestCount);
            bFinishAction = false;
            durationState = TimeState::idle;
            bCannelSequence = false;
            Log::LogByFile("LoopTest.txt",QString("开始 %1，循环数：%2").arg(sequenceAction.attribute("Panel_c")).arg(count));
            FindAction(false);
            return true;
        }
    }

    return false;
}

bool Sequence::continueLoopTest(){
    Log::LogByFile("LoopTest.txt",QString("完成循环数：%1").arg(loopTestCurrCount));
    testMgr->LoopTestFinishCycle(loopTestCurrCount);
    if (loopTestCurrCount >= loopTestCount)
        return false;
    FormatAction();
    bFinishAction = false;
    durationState = TimeState::idle;
    bCannelSequence = false;
    FindAction(false);
    loopTestCurrCount++;
    return true;
}

QStringList Sequence::getLoopTestList(){
    QStringList test;
    QDomElement root = doc.documentElement();
    for (QDomElement e = root.firstChildElement("Panel"); !e.isNull(); e = e.nextSiblingElement("Panel"))
    {
        QString panelCode = e.attribute("Panel_c");
        if (panelCode.length() > 4 && panelCode.startsWith("90"))
            test<<panelCode;
    }
    return test;
}

QStringList Sequence::onePointTestList(){
    QStringList test;
    QDomElement root = doc.documentElement();
    for (QDomElement e = root.firstChildElement("Panel"); !e.isNull(); e = e.nextSiblingElement("Panel"))
    {
        QString panelCode = e.attribute("Panel_ID");
        if (panelCode.startsWith("3"))
            test<<e.attribute("Panel_c");
    }
    return test;
}

void Sequence::lampDo(LampState state){
    switch (state) {
    case LampState::Lamp_Light_blue:
        serialMgr->serialWrite(ActionParser::ParamToByte("Lamp",1,1,65535,0));
        //serialMgr->CtrlDirectWrite(ActionParser::ParamToByte("Lamp",1,1,65535,0));
        break;
    case LampState::Lamp_Light_green:
        serialMgr->serialWrite(ActionParser::ParamToByte("Lamp",1,2,65535,0));
        //serialMgr->CtrlDirectWrite(ActionParser::ParamToByte("Lamp",1,2,65535,0));
        break;
    case LampState::Lamp_Light_orange:
        serialMgr->serialWrite(ActionParser::ParamToByte("Lamp",1,0,65535,0));
        //serialMgr->CtrlDirectWrite(ActionParser::ParamToByte("Lamp",1,0,65535,0));
        break;
    }
}

QString Sequence::getPanelName(QString panelCode){
    QDomElement root = doc.documentElement();
    qDebug()<<"getPanelName"<<panelCode;
    for (QDomElement e = root.firstChildElement("Panel"); !e.isNull(); e = e.nextSiblingElement("Panel"))
    {
        if (e.attribute("Panel_ID") == panelCode)
        {
            //qDebug()<<"ok"<<e.attribute("PanelName");
            if (ExGlobal::LanguageCode == 1 && e.hasAttribute("Panel_e"))
                return e.attribute("Panel_e");
            return e.attribute("Panel_c");
        }
    }
    return "";
}

typedef struct
{
    int V1ToolHomeX;
    int V1WorkX;
    int V1SoftHomeX;
    int V2ToolHomeX;
    int V2WorkX;
    int V2SoftHomeX;
    int V3ToolHomeX;
    int V3WorkX;
    int V3SoftHomeX;
    int VPToolHomeX;
    int VPWorkX;
    int VPSoftHomeX;
    int VDWorkX;
    int PumpToolHomeX;
    int PumpSoftHomeX;

    int VPump1;
    int VPump2;
    int VPump3;
    int VPump4;
    int VPump5;
    int VPump6;
    int VPump7;
    int VPump8;
    int VPump9;
    int VPump10;
    int VPump11;
    int VPump12;
    int VPump13;
    int VPump14;
    int VPump15;
    int16_t PID_P1;
    int16_t PID_I1;
    int16_t PID_D1;
    int16_t PID_P2;
    int16_t PID_I2;
    int16_t PID_D2;
    int16_t PID_P3;
    int16_t PID_I3;
    int16_t PID_D3;
    int16_t PID_P4;
    int16_t PID_I4;
    int16_t PID_D4;
    int16_t PID_P5;
    int16_t PID_I5;
    int16_t PID_D5;
     int16_t PID_P6;
    int16_t PID_I6;
    int16_t PID_D6;
    int16_t PID_P7;
    int16_t PID_I7;
    int16_t PID_D7;
    int16_t PID_P8;
    int16_t PID_I8;
    int16_t PID_D8;
    int16_t PID_P9;
    int16_t PID_I9;
    int16_t PID_D9;
    int16_t PID_P10;
    int16_t PID_I10;
    int16_t PID_D10;
  int16_t PID_P11;
    int16_t PID_I11;
    int16_t PID_D11;
    int16_t PID_P12;
    int16_t PID_I12;
    int16_t PID_D12;
     int16_t PID_P13;
    int16_t PID_I13;
    int16_t PID_D13;
    int16_t PID_P14;
    int16_t PID_I14;
    int16_t PID_D14;
    int16_t PID_P15;
    int16_t PID_I15;
    int16_t PID_D15;

} intParam;

void Sequence::checkParam(bool update){
    intParam param;
    if (ExGlobal::isDebug())
    {
        ReadTestProcess(QCoreApplication::applicationDirPath()+"/FLASHDXcn");
        ExGlobal::pTestModel->InitTest();
    }
    qDebug()<<"checkParam version="<<ExGlobal::tempversion();
    if (QString::compare("V2.21",ExGlobal::tempversion())>0) return;
    param.V1ToolHomeX = ExGlobal::V1ToolHomeX;
    param.V1WorkX = ExGlobal::V1WorkX;
    param.V1SoftHomeX = ExGlobal::V1SoftHomeX;
    param.V2ToolHomeX = ExGlobal::V2ToolHomeX;
    param.V2WorkX = ExGlobal::V2WorkX;
    param.V2SoftHomeX = ExGlobal::V2SoftHomeX;
    param.V3ToolHomeX = ExGlobal::V3ToolHomeX;
    param.V3WorkX = ExGlobal::V3WorkX;
    param.V3SoftHomeX = ExGlobal::V3SoftHomeX;
    param.VPToolHomeX = ExGlobal::VPToolHomeX;
    param.VPWorkX = ExGlobal::VPWorkX;
    param.VPSoftHomeX = ExGlobal::VPSoftHomeX;
    param.VDWorkX = ExGlobal::VDWorkX;
    param.PumpToolHomeX = ExGlobal::PumpToolHomeX;
    param.PumpSoftHomeX = ExGlobal::PumpSoftHomeX;

    param.VPump1 = ExGlobal::VPump1;
    param.VPump2 = ExGlobal::VPump2;
    param.VPump3 = ExGlobal::VPump3;
    param.VPump4 = ExGlobal::VPump4;
    param.VPump5 = ExGlobal::VPump5;
    param.VPump6 = ExGlobal::VPump6;
    param.VPump7 = ExGlobal::VPump7;
    param.VPump8 = ExGlobal::VPump8;
    param.VPump9 = ExGlobal::VPump9;
    param.VPump10 = ExGlobal::VPump10;
    param.VPump11 = ExGlobal::VPump11;
    param.VPump12 = ExGlobal::VPump12;
    param.VPump13 = ExGlobal::VPump13;
    param.VPump14 = ExGlobal::VPump14;
    param.VPump15 = ExGlobal::VPump15;

    param.PID_P1 = ExGlobal::PID_P1;
    param.PID_I1 = ExGlobal::PID_I1;
    param.PID_D1 = ExGlobal::PID_D1;
    param.PID_P2 = ExGlobal::PID_P2;
    param.PID_I2 = ExGlobal::PID_I2;
    param.PID_D2 = ExGlobal::PID_D2;
    param.PID_P3 = ExGlobal::PID_P3;
    param.PID_I3 = ExGlobal::PID_I3;
    param.PID_D3 = ExGlobal::PID_D3;
    param.PID_P4 = ExGlobal::PID_P4;
    param.PID_I4 = ExGlobal::PID_I4;
    param.PID_D4 = ExGlobal::PID_D4;
    param.PID_P5 = ExGlobal::PID_P5;
    param.PID_I5 = ExGlobal::PID_I5;
    param.PID_D5 = ExGlobal::PID_D5;
    param.PID_P6 = ExGlobal::PID_P6;
    param.PID_I6 = ExGlobal::PID_I6;
    param.PID_D6 = ExGlobal::PID_D6;
    param.PID_P7 = ExGlobal::PID_P7;
    param.PID_I7 = ExGlobal::PID_I7;
    param.PID_D7 = ExGlobal::PID_D7;
    param.PID_P8 = ExGlobal::PID_P8;
    param.PID_I8 = ExGlobal::PID_I8;
    param.PID_D8 = ExGlobal::PID_D8;
     param.PID_P9 = ExGlobal::PID_P9;
    param.PID_I9 = ExGlobal::PID_I9;
    param.PID_D9 = ExGlobal::PID_D9;
   param.PID_P10 = ExGlobal::PID_P10;
    param.PID_I10 = ExGlobal::PID_I10;
    param.PID_D10 = ExGlobal::PID_D10;
  param.PID_P11 = ExGlobal::PID_P11;
    param.PID_I11 = ExGlobal::PID_I11;
    param.PID_D11 = ExGlobal::PID_D11;
    param.PID_P12 = ExGlobal::PID_P12;
    param.PID_I12 = ExGlobal::PID_I12;
    param.PID_D12 = ExGlobal::PID_D12;
    param.PID_P13 = ExGlobal::PID_P13;
    param.PID_I13 = ExGlobal::PID_I13;
    param.PID_D13 = ExGlobal::PID_D13;
   param.PID_P14 = ExGlobal::PID_P14;
    param.PID_I14 = ExGlobal::PID_I14;
    param.PID_D14 = ExGlobal::PID_D14;
     param.PID_P15 = ExGlobal::PID_P15;
    param.PID_I15 = ExGlobal::PID_I15;
    param.PID_D15 = ExGlobal::PID_D15;

    QByteArray byteParam;
    byteParam.append((char*)&param,sizeof(intParam)-2);

    QByteArray send ;


    if (update)
        send=ActionParser::ParamArrToByte(byteParam,2);
      //  serialMgr->serialWrite(ActionParser::ParamArrToByte(byteParam,2));
     else
        send=ActionParser::ParamArrToByte(byteParam,1);
       // serialMgr->serialWrite(ActionParser::ParamArrToByte(byteParam,1));
   // currOrder = send[7];
    serialMgr->serialWrite(send);
     qDebug()<<"checkParam:+VPump1+VPump2+VPump3+VPump4+VPump5+VPump14+VPump15"<<param.VPump1<<"::"<<param.VPump2<<"::"<<param.VPump3<<"::"<<param.VPump4<<"::"<<param.VPump5<<"::"<<param.VPump14<<"::"<<param.VPump15<<"::::::::::";
    qDebug()<<"checkParam:+V1ToolHomeX+V2ToolHomeX+V3ToolHomeX"<<param.V1ToolHomeX<<"::"<<param.V2ToolHomeX<<"::"<<param.V3ToolHomeX<<"::::::::::";

}

void Sequence::updateParam(QByteArray param){
    qDebug()<<"updateParam:"<<QString::number(param[11],16);
    if (param[11] == '\x1' && QString::number(param[9]).toInt() == sizeof(intParam)){
        intParam iparam;
        memcpy((char *)&iparam,param.mid(12),sizeof(intParam));
        qDebug()<<"V1="<<iparam.V1WorkX<<iparam.V1ToolHomeX<<iparam.V1SoftHomeX;
        ExGlobal::updateCaliParam("V1ToolHomeX",iparam.V1ToolHomeX);
        ExGlobal::updateCaliParam("V1WorkX",iparam.V1WorkX);
        ExGlobal::updateCaliParam("V1SoftHomeX",iparam.V1SoftHomeX);
        ExGlobal::updateCaliParam("V2ToolHomeX",iparam.V2ToolHomeX);
        ExGlobal::updateCaliParam("V2WorkX",iparam.V2WorkX);
        ExGlobal::updateCaliParam("V2SoftHomeX",iparam.V2SoftHomeX);
        ExGlobal::updateCaliParam("V3ToolHomeX",iparam.V3ToolHomeX);
        ExGlobal::updateCaliParam("V3WorkX",iparam.V3WorkX);
        ExGlobal::updateCaliParam("V3SoftHomeX",iparam.V3SoftHomeX);
        ExGlobal::updateCaliParam("VPToolHomeX",iparam.VPToolHomeX);
        ExGlobal::updateCaliParam("VPWorkX",iparam.VPWorkX);
        ExGlobal::updateCaliParam("VPSoftHomeX",iparam.VPSoftHomeX);
        ExGlobal::updateCaliParam("VDWorkX",iparam.VDWorkX);
        ExGlobal::updateCaliParam("PumpToolHomeX",iparam.PumpToolHomeX);
        ExGlobal::updateCaliParam("PumpSoftHomeX",iparam.PumpSoftHomeX);
        qDebug()<<"updateParam:+V1ToolHomeX+V2ToolHomeX+V3ToolHomeX"<<iparam.V1ToolHomeX<<"::"<<iparam.V2ToolHomeX<<"::"<<iparam.V3ToolHomeX<<"::::::::::";


        ExGlobal::updateCaliParam("VPump1",iparam.VPump1);
        ExGlobal::updateCaliParam("VPump2",iparam.VPump2);
        ExGlobal::updateCaliParam("VPump3",iparam.VPump3);
        ExGlobal::updateCaliParam("VPump4",iparam.VPump4);
        ExGlobal::updateCaliParam("VPump5",iparam.VPump5);
        ExGlobal::updateCaliParam("VPump6",iparam.VPump6);
        ExGlobal::updateCaliParam("VPump7",iparam.VPump7);
        ExGlobal::updateCaliParam("VPump8",iparam.VPump8);
        ExGlobal::updateCaliParam("VPump9",iparam.VPump9);
        ExGlobal::updateCaliParam("VPump10",iparam.VPump10);
        ExGlobal::updateCaliParam("VPump11",iparam.VPump11);
        ExGlobal::updateCaliParam("VPump12",iparam.VPump12);
        ExGlobal::updateCaliParam("VPump13",iparam.VPump13);
        ExGlobal::updateCaliParam("VPump14",iparam.VPump14);
        ExGlobal::updateCaliParam("VPump15",iparam.VPump15);

        ExGlobal::updateCaliParam("PID_P1",iparam.PID_P1);
        ExGlobal::updateCaliParam("PID_I1",iparam.PID_I1);
        ExGlobal::updateCaliParam("PID_D1",iparam.PID_D1);
        ExGlobal::updateCaliParam("PID_P2",iparam.PID_P2);
        ExGlobal::updateCaliParam("PID_I2",iparam.PID_I2);
        ExGlobal::updateCaliParam("PID_D2",iparam.PID_D2);
        ExGlobal::updateCaliParam("PID_P3",iparam.PID_P3);
        ExGlobal::updateCaliParam("PID_I3",iparam.PID_I3);
        ExGlobal::updateCaliParam("PID_D3",iparam.PID_D3);
        ExGlobal::updateCaliParam("PID_P4",iparam.PID_P4);
        ExGlobal::updateCaliParam("PID_I4",iparam.PID_I4);
        ExGlobal::updateCaliParam("PID_D4",iparam.PID_D4);
        ExGlobal::updateCaliParam("PID_P5",iparam.PID_P5);
        ExGlobal::updateCaliParam("PID_I5",iparam.PID_I5);
        ExGlobal::updateCaliParam("PID_D5",iparam.PID_D5);

        ExGlobal::updateCaliParam("PID_P6",iparam.PID_P6);
        ExGlobal::updateCaliParam("PID_I6",iparam.PID_I6);
        ExGlobal::updateCaliParam("PID_D6",iparam.PID_D6);
        ExGlobal::updateCaliParam("PID_P7",iparam.PID_P7);
        ExGlobal::updateCaliParam("PID_I7",iparam.PID_I7);
        ExGlobal::updateCaliParam("PID_D7",iparam.PID_D7);
        ExGlobal::updateCaliParam("PID_P8",iparam.PID_P8);
        ExGlobal::updateCaliParam("PID_I8",iparam.PID_I8);
        ExGlobal::updateCaliParam("PID_D8",iparam.PID_D8);
        ExGlobal::updateCaliParam("PID_P9",iparam.PID_P9);
        ExGlobal::updateCaliParam("PID_I9",iparam.PID_I9);
        ExGlobal::updateCaliParam("PID_D9",iparam.PID_D9);
       ExGlobal::updateCaliParam("PID_P10",iparam.PID_P10);
        ExGlobal::updateCaliParam("PID_I10",iparam.PID_I10);
        ExGlobal::updateCaliParam("PID_D10",iparam.PID_D10);

        ExGlobal::updateCaliParam("PID_P11",iparam.PID_P11);
        ExGlobal::updateCaliParam("PID_I11",iparam.PID_I11);
        ExGlobal::updateCaliParam("PID_D11",iparam.PID_D11);
        ExGlobal::updateCaliParam("PID_P12",iparam.PID_P12);
        ExGlobal::updateCaliParam("PID_I12",iparam.PID_I12);
        ExGlobal::updateCaliParam("PID_D12",iparam.PID_D12);
        ExGlobal::updateCaliParam("PID_P13",iparam.PID_P13);
        ExGlobal::updateCaliParam("PID_I13",iparam.PID_I13);
        ExGlobal::updateCaliParam("PID_D13",iparam.PID_D13);
        ExGlobal::updateCaliParam("PID_P14",iparam.PID_P14);
        ExGlobal::updateCaliParam("PID_I14",iparam.PID_I14);
        ExGlobal::updateCaliParam("PID_D14",iparam.PID_D14);
        ExGlobal::updateCaliParam("PID_P15",iparam.PID_P15);
        ExGlobal::updateCaliParam("PID_I15",iparam.PID_I15);
        ExGlobal::updateCaliParam("PID_D15",iparam.PID_D15);


        qDebug()<<"updateParam:+VPump1+VPump2+VPump3+VPump4+VPump5+VPump6"<<iparam.VPump1<<"::"<<iparam.VPump2<<"::"<<iparam.VPump3<<"::"<<iparam.VPump4<<"::"<<iparam.VPump5<<"::"<<iparam.VPump6<<"::::::::::";


    }
}

void Sequence::setFocus(){
    ImageAnalysis::FocusSet(ExGlobal::FocusX,ExGlobal::FocusY,ExGlobal::FocusWidth,ExGlobal::FocusHeight);
}

void Sequence::setLight(){
    ImageAnalysis::LightSet(ExGlobal::LightCX,ExGlobal::LightCY,ExGlobal::LightR);
}


void Sequence::clear_data(int value)
{
    QByteArray send ;
    send=ActionParser::Clear_board_data(1);
    serialMgr->serialWrite(send);
}
