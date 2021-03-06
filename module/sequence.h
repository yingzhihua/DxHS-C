#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QObject>
#include <QtCore>
#include <QtXml>
#include "serialmgr.h"
#include "imageanalysis.h"
#include "entity.h"
#include "qrcoder.h"
#include "printmgr.h"

#include "log.h"
#include "dao/testmgr.h"
#include "cvcapture.h"
#include "tcamera.h"
#include "udpserver.h"

class Sequence : public QObject
{
    Q_OBJECT
public:
    explicit Sequence(QObject *parent = nullptr);    
    static Sequence *getPtr();
    enum SequenceId{
        Sequence_Idle,
        Sequence_SelfCheck,
        Sequence_OpenBox,
        Sequence_ErrOpenBox,
        Sequence_CloseBox,
        Sequence_DoorToWork,
        Sequence_DoorFromWork,
        Sequence_CannelTest,
        Sequence_Test,
        Sequence_LoopTest,
        Sequence_SimpleAction,
        Sequence_QrDecode,
        Sequence_Pierce,
        Sequence_camView
    };
    Q_ENUM(SequenceId)

    enum class SequenceResult{
        Result_ok,
        Result_fail,
        Result_Simple_ok,
        Result_SelfCheck_ok,
        Result_SelfCheck_err1,
        Result_SelfCheck_err2,
        Result_OpenBox_ok,
        Result_OpenBox_err,
        Result_CloseBox_ok,
        Result_CloseBox_err,
        Result_DoorToWork_ok,
        Result_CannelTest_ok,
        Result_Test_HandleData,
        Result_Test_finish,
        Result_Test_unfinish,
        Result_LoopTest_finish,
        Result_CannelLoopTest_ok,
        Result_Print_finish,
        Result_Print_Error,
        Result_Test_DataErr,
        Result_Test_ProcessErr,
        Result_Test_DryFillErr,
        Result_Box_Valid,        
        Result_Box_Invalid,
        Result_Pause,
        Result_NULL
    };
    Q_ENUM(SequenceResult)

    enum class TimeState{
        idle,
        wait,
        running
    };
    Q_ENUM(TimeState)

    enum class LampState{
        Lamp_Light_blue,
        Lamp_Light_green,
        Lamp_Light_orange
    };
    Q_ENUM(LampState)

    enum class StageState{
        Stage_selfcheck,
        Stage_login,
        Stage_idle,
        Stage_ready,
        Stage_test
    };
    Q_ENUM(StageState)

    Q_PROPERTY(bool door READ readDoorState NOTIFY doorStateChanged)
    Q_PROPERTY(bool box READ readBoxState NOTIFY boxStateChanged)    

    Q_INVOKABLE bool sequenceDo(SequenceId id);
    Q_INVOKABLE void lampDo(LampState state);
    Q_INVOKABLE QString getCurrTestTime();
    Q_INVOKABLE bool actionDo(QString device, int value, int param1, int param2, int param3);
    Q_INVOKABLE void sequenceCancel();
    Q_INVOKABLE void sequenceContinue();
    Q_INVOKABLE QString sequenceMessage();
    Q_INVOKABLE void sequenceSetPanel(QString PanelName);
    Q_PROPERTY(QStringList TestList READ getTestList NOTIFY testListChanged)
    Q_PROPERTY(QStringList LoopTestList READ getLoopTestList NOTIFY testListChanged)
    Q_INVOKABLE bool sequenceInit();
    Q_INVOKABLE void openErr(QString err){emit errOccur(err);}
    QStringList getTestList();
    QStringList getLoopTestList();
    Q_INVOKABLE QStringList onePointTestList();
    bool readBoxState(){return bBoxState;}
    bool readDoorState(){return bDoorState;}

    Q_INVOKABLE int getAbs();
    Q_INVOKABLE bool setAbs(int value);
    Q_INVOKABLE int getGain();
    Q_INVOKABLE bool setGain(int value);    
    Q_INVOKABLE int getWhiteBalance();
    Q_INVOKABLE bool setWhiteBalance(int value);

    Q_INVOKABLE bool startView(int id);
    Q_INVOKABLE bool stopView();
    Q_INVOKABLE bool saveView();
    Q_INVOKABLE void lxDebug();
    Q_INVOKABLE void showAnaImg(int type,int light);
    Q_INVOKABLE void qrSet(bool bopenlight, bool scale, bool handlimage, int bin,int pox);

    Q_INVOKABLE double getDefinition();
    Q_INVOKABLE double getDefinition1(){return imageAna->GetDefinition(camera->getyData(),camera->getImageType());}
    Q_INVOKABLE double getDefinition2(){return imageAna->GetDefinition2(camera->getyData(),camera->getImageType());}
    Q_INVOKABLE double getDefinition3(){return imageAna->GetDefinition3(camera->getyData(),camera->getImageType());}
    Q_INVOKABLE double getLight(){return imageAna->GetMeanLight(camera->getyData(),camera->getImageType());}
    Q_INVOKABLE double getLLight(){return imageAna->GetLLight(camera->getyData(),camera->getImageType());}
    Q_INVOKABLE double getCircSize(){return imageAna->GetCircularSize(camera->getyData(),camera->getImageType());}
    Q_INVOKABLE bool validBox(){return bValidBox;}

    Q_PROPERTY(int fan1Speed READ fan1Speed NOTIFY fan1SpeedChanged)
    Q_PROPERTY(int fan2Speed READ fan2Speed NOTIFY fan2SpeedChanged)
    Q_PROPERTY(int fan3Speed READ fan3Speed NOTIFY fan3SpeedChanged)
    Q_INVOKABLE void autoFocus();
    Q_INVOKABLE void startLight();

    Q_INVOKABLE bool printTest();    

    Q_INVOKABLE bool isTesting(){qDebug()<<"isTesting"<<currSequenceId;return currSequenceId == SequenceId::Sequence_Test;}
    Q_INVOKABLE bool isIdle(){return currSequenceId == SequenceId::Sequence_Idle;}
    Q_INVOKABLE void changeTitle(QString title);
    Q_INVOKABLE void setTitle(QString titleid);
    Q_INVOKABLE void hideTitle(bool hide);
    Q_INVOKABLE void updateFooter(bool setup, bool home, bool data){emit footerNotify(setup,home,data);}

    Q_INVOKABLE bool loopTest(QString testName, int count);
    Q_INVOKABLE bool isLoopTesting(){return currSequenceId == SequenceId::Sequence_LoopTest;}

    Q_INVOKABLE int boxParam(){return boxparam;}    
    Q_INVOKABLE int onePointPanelIndex(){return OnePointPanelIndex;}
    Q_INVOKABLE void setOnePointPanelIndex(int index);
     void checkParam(bool update);
    Q_INVOKABLE bool doorError(){return bDoorState!=bDoorState2;}
    Q_INVOKABLE void clear_data(int value);

    static QString getPanelName(QString panelCode);
    Q_PROPERTY(StageState uiStage READ readStage WRITE setStage NOTIFY stageChanged)
    StageState readStage(){return stage;}
    void setStage(StageState uiStage){stage = uiStage;}
    Q_INVOKABLE void setFocus();
    Q_INVOKABLE void setLight();
    Q_INVOKABLE void setViewType(int viewType){camera->setPreviewType(viewType);}    
    bool printPDFResult();
signals:
    void sequenceFinish(SequenceResult result);
    void processFinish(int total,int finish);
    void panelNameChanged();
    void testListChanged();
    void doorStateChanged();
    void doorKeyPress();
    void boxStateChanged();
    void errOccur(QString error);
    void callQmlRefeshView();
    void callRefeshView(QImage img);
    void callQmlRefeshQrImg();
    void refreshLightValue();
    void callQmlRefeshAnaMainImg();
    void callQmlRefeshData(int index, QVector<int> item,QVector<int> value);
    void qrDecode(QString info);
    void fan1SpeedChanged();
    void fan2SpeedChanged();
    void fan3SpeedChanged();
    void autoFocusNotify(int status, int value);
    void titleNotify(int titleparam, QString title);
    void footerNotify(bool setup, bool home, bool data);
    void stageChanged();

public slots:
    void SequenceTimeout();
    void WaitSequenceTimeout();
    void TestSecondTimeout();
    void ActionFinish(QByteArray data);
    void PrintFinish(QByteArray data);
    void errFinish(QByteArray data);
    void CameraView(QImage img);    
    void errReceive(ERROR_CODE code);
    void TestDataHandleFinish(int result);

private:
    bool ReadTestProcess(QString panel);
    bool WriteTestProcess(QString panel);
    bool ReadMask(QString mask);
    bool DoAction(QDomElement action,bool isChild);
    bool FindAction(bool bFinishAction);
    int CalSteps(QDomElement element);
    bool FormatAction();
    void FinishSequence();    
    void setSenorState(char char1, char char2);

    QDomElement sequenceAction;
    QTimer *timer;
    QTimer *waitNextSequence;
    QTimer *testSecondTime;
    SequenceId currSequenceId;
    SequenceId nextSequenceId;
    int waitCount;
    QString message;
    SerialMgr *serialMgr;
    UdpServer *udpServer;

    cvCapture *cvcap;
    TestMgr *testMgr;
    printmgr *printer;

    //ImageCapture *imageCapture;
    TCamera *camera;

    bool bFinishAction;
    TimeState durationState;
    int nDuration;
    int nPreTime;
    int nTestSecond;
    bool firstCapture;    
    bool bCannelSequence;
    bool bDoorState;
    bool bDoorState2;
    bool bBoxState;
    bool bValidBox;

    ImageAnalysis *imageAna;

    QRcoder *qr;
    QList<action> actList;
    bool listNextAction(bool first);
    void qrDect();
    int decodeQr(QString strQr);
    void PierceDect();
    void SwitchDoor(bool error);
    void DoorToWork(bool toWork);
    bool bQrOpenLight = true;
    bool bAutoFocus = false;
    bool bFocused = false;
    bool bCaptureLight = false;

    int t_fan1Speed;
    int t_fan2Speed;
    int t_fan3Speed;
    int fan1Speed(){return t_fan1Speed;}
    int fan2Speed(){return t_fan2Speed;}
    int fan3Speed(){return t_fan3Speed;}
    void fan1SetSpeed(int speed);
    void fan2SetSpeed(int speed){t_fan2Speed = speed;emit fan2SpeedChanged();}
    void fan3SetSpeed(int speed){t_fan3Speed = speed;emit fan3SpeedChanged();}

    QString title;
    int titleparam;
    int boxparam;

    int loopTestCount;
    int loopTestCurrCount;
    QString loopTestName;
    bool continueLoopTest();

    bool dirctAction(QString device, int value, int param1, int param2, int param3);
    void updateParam(QByteArray param);

    double dryMeanValue,fillMeanValue;
    bool masked;
    StageState stage;
    int OnePointPanelIndex;
};

#endif // SEQUENCE_H
