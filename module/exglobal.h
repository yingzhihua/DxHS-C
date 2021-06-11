#ifndef EXGLOBAL_H
#define EXGLOBAL_H

#include <QObject>
#include <QTranslator>
#include <QApplication>
#include <QDebug>

#include "dao/testmodel.h"
#include "dao/logmodel.h"
#include "dao/testresultmodel.h"
#include "dao/usermodel.h"
#include "dao/wifimodel.h"
#include "dao/usbmodel.h"


class NetIpConnect : public QObject
{
    Q_OBJECT
public:
    explicit NetIpConnect(QObject *parent = nullptr);
    void setParam(QString ipc,QString maskc,QString gatec){ip = ipc;mask=maskc;gate=gatec;}
    void setMode(int mode_){mode = mode_;}

signals:
    void Progress(int val);
    void Finish();

public slots:
    void start();

private:
    QString ip;
    QString mask;
    QString gate;
    int mode;
};


typedef int (*StartFun)();
class ThreadWorker : public QObject
{
    Q_OBJECT
public:
    explicit ThreadWorker(int(*start)()){fun=start;}

signals:
    void Finish(int val);

public slots:
    void start();

    private:
    StartFun fun;
};

class ExGlobal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QString displayUser READ displayUser WRITE setDisplayUser NOTIFY userChanged)
    Q_PROPERTY(QString panelName READ panelName NOTIFY panelNameChanged)
    Q_PROPERTY(QString panelCode READ panelCode NOTIFY panelCodeChanged)
    Q_PROPERTY(QString panelId READ panelId NOTIFY panelIdChanged)
    Q_PROPERTY(QString sampleCode READ sampleCode WRITE setSampleCode NOTIFY sampleCodeChanged)
    Q_PROPERTY(QString sampleInfo READ sampleInfo WRITE setSampleInfo NOTIFY sampleInfoChanged)
    Q_PROPERTY(QString sampleRemark READ sampleRemark WRITE setSampleRemark NOTIFY sampleRemarkChanged)
    Q_PROPERTY(QStringList serialPort READ serialPort NOTIFY serialPortChanged)
    Q_PROPERTY(bool debug READ isDebug WRITE setDebug NOTIFY debugChanged)
    Q_PROPERTY(QString sysName READ sysName WRITE setSysName NOTIFY sysNameChanged)
    Q_PROPERTY(QString hospitalName READ hospitalName WRITE setHospitalName NOTIFY hospitalNameChanged)
    Q_PROPERTY(int printType READ printType WRITE setPrintType NOTIFY printTypeChanged)
    Q_PROPERTY(QString sysLanguageName READ sysLanguageName NOTIFY sysLanguageNameChanged)
    Q_PROPERTY(int sysLanguageCode READ sysLanguageCode WRITE setSysLanguageCode NOTIFY sysLanguageCodeChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QString buildversion READ buildversion NOTIFY versionChanged)
    Q_PROPERTY(QString tempversion READ tempversion NOTIFY versionChanged)
    Q_PROPERTY(QString ctrlversion READ ctrlversion NOTIFY versionChanged)    
    Q_PROPERTY(int lockTime READ lockTime WRITE setLockTime NOTIFY lockTimeChanged)
    Q_PROPERTY(bool lockscreenOpen READ lockscreenOpen WRITE setLockscreenOpen NOTIFY lockscreenOpenChanged)
    Q_PROPERTY(int timezone READ timezone WRITE settimezone NOTIFY timezonechanged)
    Q_PROPERTY(bool timezoneopen READ timezoneopen WRITE settimezoneopen NOTIFY timezoneopenchanged)
    Q_PROPERTY(QString adminPassword READ adminPassword WRITE setAdminPassword NOTIFY adminPasswordChanged)
    Q_PROPERTY(bool childImage READ childImage WRITE setChildImage NOTIFY childImageChanged)
    Q_PROPERTY(int autoFocus READ autoFocus WRITE setAutoFocus NOTIFY autoFocusChanged)
    Q_PROPERTY(int qrCode READ qrCode WRITE setQrCode NOTIFY qrCodeChanged)
    Q_PROPERTY(QList<int> uiParam READ getUIParam NOTIFY uiParamChanged)
    Q_PROPERTY(int pubParam READ getPubParam WRITE setPubParam NOTIFY pubparamchanged)
public:
    enum UI_PARAM:int16_t{
        UI_MAIN_MENU_FONT,
        UI_MAIN_MENU_HORIZONTALOFFSET,
        UI_LOGIN_SUBMIT_FONT,
        UI_IDLE_EXITLOGIN_FONT,
        UI_IDLE_SHUTDOWN_FONT,
        UI_BOXREADY_BUTTOM_PROMPT_FONT,
        UI_BOXREADY_TEST_CANCEL_FONT,
        UI_PARAM_LENGTH
    };
    Q_ENUM(UI_PARAM)

    explicit ExGlobal(QObject *parent = nullptr);    
    static ExGlobal *getPtr();
    static void exInit();

    QString user() {return User;}
    QString displayUser() {return DisplayUser;}
    void setUser(const QString &user){User = user;emit userChanged();}
    void setDisplayUser(const QString &user){DisplayUser = user;emit userChanged();}
    static QString panelName(){qDebug()<<"getpanelName"<<t_panelName;return t_panelName;}
    static int SysCommand(QString command);
    static void AsynCommand(QString command);
    static void setPanelName(const QString &panelName){qDebug()<<"setpanelName"<<panelName;t_panelName = panelName;}
    static QString panelCode(){return t_panelCode;}
    static QString panelId(){return t_panelId;}
    static QString panelCodeEx(){return "Lot# "+t_panelCode;}
    static void setPanelCode(const QString &panelCode){t_panelCode = panelCode;}
    static void setPanelId(const QString &panelId){t_panelId = panelId;}
    static QString sampleCode(){return t_sampleCode;}
    void setSampleCode(const QString &sampleCode);
    static QString sampleInfo(){return t_sampleInfo;}
    void setSampleInfo(const QString &sampleInfo){t_sampleInfo = sampleInfo;emit sampleInfoChanged();}
    static QString sampleRemark(){return t_sampleRemark;}
    void setSampleRemark(const QString &sampleRemark){t_sampleRemark = sampleRemark;emit sampleRemarkChanged();}
    static QString reagentBox();
    static void setReagentBox(const QString &reagentBoxType);
    static QStringList serialPort();
    static bool isDebug(){return test;}
    static void setDebug(bool debug){test = debug;}
    QString sysName(){return SysName;}
    void setSysName(const QString &name){updateTextParam("SysName",name);emit sysNameChanged();}
    QString hospitalName(){return HospitalName;}
    void setHospitalName(const QString &name){updateTextParam("HospitalName",name);emit hospitalNameChanged();}
    int printType(){return PrintType;}
    void setPrintType(int printType){updateCaliParam("PrintType",printType);emit printTypeChanged();}
    QString adminPassword(){return AdminPassword;}
    void setAdminPassword(const QString &password){updateTextParam("AdminPassword",password);emit adminPasswordChanged();}
    static QString sysLanguageName();
    int sysLanguageCode(){return LanguageCode;}
    void setSysLanguageCode(int code);
    static QString version(){return t_version;}
    static QString buildversion();
    static QString tempversion(){return temp_version;}
    static QString ctrlversion(){return ctrl_version;}
    static void settempversion(const QString &version){temp_version = version;}
    static void setctrlversion(const QString &version){ctrl_version = version;}
    int lockTime(){return getCaliParam("LockScreenTime");}
    void setLockTime(int time){updateCaliParam("LockScreenTime",time);updateLockTime();emit lockTimeChanged();}
    bool lockscreenOpen(){return LockScreenOpen == 1;}
    void setLockscreenOpen(bool bopen){updateCaliParam("LockScreenOpen",bopen?1:0);updateLockTime();emit lockscreenOpenChanged();}
    int timezone(){return getCaliParam("TimeZone");}
    void settimezone(int zone){updateCaliParam("TimeZone",zone);updatetimezone();emit timezonechanged();}
    bool timezoneopen(){return TimeZoneOpen == 1;}
    void settimezoneopen(bool bopen){updateCaliParam("TimeZoneOpen",bopen?1:0);bopen?system("timedatectl set-ntp true"):system("timedatectl set-ntp false");updatetimezone();emit timezoneopenchanged();}


    bool childImage(){return bChildImage;}
    void setChildImage(bool set){bChildImage = set;}
    bool autoFocus(){return AutoFocus;}
    void setAutoFocus(int set){updateCaliParam("AutoFocus",set);emit autoFocusChanged();}
    int qrCode(){return QrCode;}
    void setQrCode(int set){updateCaliParam("QrCode",set);emit qrCodeChanged();}

    void setPubParam(int param){pubparam = param;}
    int getPubParam(){return pubparam;}

    static uchar* getReagentBox(QString boxCode);
    Q_INVOKABLE static QString getPosName(int pos);
    static QString getPosFullName(int pos);
    static QString getItemCode(int itemid);
    static QString ItemCodeToFullName(QString itemCode);
    Q_INVOKABLE static int getLJTemp(int param);
    Q_INVOKABLE static int getPumpParam(int PumpName);
    Q_INVOKABLE static void updateCaliParam(const QString &caliName, int caliValue);
    Q_INVOKABLE static int getCaliParam(const QString &caliName);
    Q_INVOKABLE static void updateTextParam(const QString &caliName, QString caliValue);
    Q_INVOKABLE static QString getTextParam(const QString &caliName);
    Q_INVOKABLE static QString getIP();
    Q_INVOKABLE static QString getWIFIIP();
    Q_INVOKABLE static QString getMac();
    Q_INVOKABLE static QString getSerial();
    Q_INVOKABLE static QStringList getNetWork();
    Q_INVOKABLE static void setNetWork(const QString &ip,const QString &mask,const QString &gate);
    Q_INVOKABLE void setNetIp(const QString &ip,const QString &mask,const QString &gate);
    Q_INVOKABLE bool setTime(QString time);
    Q_INVOKABLE static void exClose();
    Q_INVOKABLE static QStringList getPosNameArray();
    Q_INVOKABLE static int getItemCT(int Itemid);    
    Q_INVOKABLE static QList<int> getBoxItemList();
    static QList<int> getBoxItemListByName(QString name);
    Q_INVOKABLE static QStringList getBoxItemName();
    Q_INVOKABLE static QStringList getSampleTypeArr();
    static QString getSampleTypeName(int type);
    Q_INVOKABLE static void setSampleType(int type){SampleType = type;}
    Q_INVOKABLE static int getSampleType(){return SampleType;}
    Q_INVOKABLE static QString getCurrSampleName();

    Q_INVOKABLE void qmlGlobalMessage(int code){GlobalMessage(code);}

    Q_INVOKABLE static int projectMode(){return ProjectMode;} //0:临时版本；1：正式版本; 2:注册版本
    Q_INVOKABLE void panelBoxIndexAddOne();
    Q_INVOKABLE int dataEntry(){return DataEntry;}
    Q_INVOKABLE void setDataEntry(int entry){DataEntry = entry;}
    Q_INVOKABLE static QString boxSerial(){return t_BoxSerial;}
    static void setBoxSerial(QString serial){t_BoxSerial = serial;}
    Q_INVOKABLE static int diskCheck();
    Q_INVOKABLE static void setHiddenFunction(){HiddenFunction = true;}
    Q_INVOKABLE static bool getHiddenFunction(){return HiddenFunction;}
    Q_INVOKABLE static void set_backlight(float value);
    Q_INVOKABLE static float read_backlight(void);
    Q_INVOKABLE static QString diskSpac();
    static int BlackLightPWM;

    static void setLogin(){bLogin = true;}
    Q_INVOKABLE static bool login(){return bLogin;}
    static void MethodToThread(
            int(*start)(),
            const QObject *receiver = nullptr,
            const char *method = nullptr);

    static int contentWidth;
    static int contentHeight;
    static int V1WorkX;
    static int V2WorkX;
    static int V3WorkX;
    static int VPWorkX;    
    static int V1WorkOffset;
    static int V2WorkOffset;
    static int V3WorkOffset;
    static int VPWorkOffset;    
    static int V1SoftHomeX;
    static int V2SoftHomeX;
    static int V3SoftHomeX;
    static int VPSoftHomeX;
    static int PumpSoftHomeX;
    static int V1SoftHomeOffset;
    static int V2SoftHomeOffset;
    static int V3SoftHomeOffset;
    static int VPSoftHomeOffset;
    static int PumpSoftHomeOffset;
    static int V1ToolHomeX;
    static int V2ToolHomeX;
    static int V3ToolHomeX;
    static int VPToolHomeX;
    static int PumpToolHomeX;
    static int VDWorkX;
    static int VDSoftHomeX;

    static int LockScreenTime;
    static int lockscreen_time;
    static int LockScreenOpen;
    static int LanguageCode;
    static int PanelBoxIndex;

    static int TimeZone_s;
    static int TimeZoneOpen;
    static int TimeZone;

    static int CamAbs;
    static int CamGain;
    static int CamFocus;
    static int CamWhiteBlance;

    static int QrX1;
    static int QrY1;
    static int QrX2;
    static int QrY2;
    static int QrX3;
    static int QrY3;
    static int QrX4;
    static int QrY4;
    static int poxValue;

    static int VPump1;
    static int VPump2;
    static int VPump3;
    static int VPump4;
    static int VPump5;
    static int VPump6;
    static int VPump7;
    static int VPump8;
    static int VPump9;
    static int VPump10;
    static int VPump11;
    static int VPump12;
    static int VPump13;
    static int VPump14;
    static int VPump15;

    static uint16_t PID_P1;
    static uint16_t PID_I1;
    static uint16_t PID_D1;
    static uint16_t PID_P2;
    static uint16_t PID_I2;
    static uint16_t PID_D2;
    static uint16_t PID_P3;
    static uint16_t PID_I3;
    static uint16_t PID_D3;
    static uint16_t PID_P4;
    static uint16_t PID_I4;
    static uint16_t PID_D4;
    static uint16_t PID_P5;
    static uint16_t PID_I5;
    static uint16_t PID_D5;
    static uint16_t PID_P6;
    static uint16_t PID_I6;
    static uint16_t PID_D6;
    static uint16_t PID_P7;
    static uint16_t PID_I7;
    static uint16_t PID_D7;
    static uint16_t PID_P8;
    static uint16_t PID_I8;
    static uint16_t PID_D8;
    static uint16_t PID_P9;
    static uint16_t PID_I9;
    static uint16_t PID_D9;
    static uint16_t PID_P10;
    static uint16_t PID_I10;
    static uint16_t PID_D10;
    static uint16_t PID_P11;
    static uint16_t PID_I11;
    static uint16_t PID_D11;
    static uint16_t PID_P12;
    static uint16_t PID_I12;
    static uint16_t PID_D12;
    static uint16_t PID_P13;
    static uint16_t PID_I13;
    static uint16_t PID_D13;
    static uint16_t PID_P14;
    static uint16_t PID_I14;
    static uint16_t PID_D14;
    static uint16_t PID_P15;
    static uint16_t PID_I15;
    static uint16_t PID_D15;

    static TestModel *pTestModel;
    static LogModel *pLogModel;
    static TestResultModel *pTestResultModel;
    static UserModel *pUserModel;
    static WifiModel *pWifiModel;
    static UsbModel *pUsbModel;

    static QMap<int, QString> AssayItem;
    static QMap<int, QString> ItemCode;
    static QMap<QString, QString> ItemName;
    static QMap<QString, QString> ItemFullName;
    static QHash<int, int> ItemCT;

    static QString AdminPassword;
    static QString User;
    static QString DisplayUser;
    static int UserType;
    static bool bChildImage;
    static int AutoFocus;
    static int QrCode;
    static int ProjectMode;
    static int MachineMode;
    static bool HiddenFunction;
    static int ClearMode;
    static int LiquidsHeight;
    static int DryWet;
    static int ClearWidth;
    static int FocusX;
    static int FocusY;
    static int FocusWidth;
    static int FocusHeight;
    static int LightCX;
    static int LightCY;
    static int LightR;
    static int DoorOut;
    static int DataEntry;
    static unsigned char *bufrgb;
    static unsigned char *hbufrgb;

    static QString SysName;
    static QString HospitalName;
    static QString DataVersion;
    static int PrintType;
    static QTranslator tor;
    static QApplication *app;    
    static void Translator(int language);
    static void setUIParam(int language);
    static QString screenCapture();

    static QString DemoPanelCode;
    static QString OnePointPanelCode;
    static int SampleType;
    static QDateTime validDateTime;
    static QList<int> UIParam;
    QList<int> getUIParam(){return UIParam;}
signals:
    void userChanged();
    void panelNameChanged();
    void panelCodeChanged();
    void panelIdChanged();
    void sampleCodeChanged();
    void sampleInfoChanged();
    void sampleRemarkChanged();
    void serialPortChanged();
    void debugChanged();
    void projectModeChanged();
    void sysNameChanged();
    void hospitalNameChanged();
    void printTypeChanged();
    void adminPasswordChanged();
    void sysLanguageNameChanged();
    void sysLanguageCodeChanged();
    void childImageChanged();
    void autoFocusChanged();
    void qrCodeChanged();
    void versionChanged();
    void lockTimeChanged();
    void lockscreenOpenChanged();
    void timezonechanged();
    void timezoneopenchanged();
    void exglobalMessage(int code);
    void uiParamChanged();
    void netFinish(int result);
    void pubparamchanged();

public slots:
    void ConnectFinish(int result);
    void DiskCheckFinish(int result);

private:
    static QString t_panelCode;
    static QString t_panelName;
    static QString t_panelId;
    static QString t_sampleCode;
    static QString t_sampleInfo;    
    static QString t_sampleRemark;
    static QString BoxCode;
    static QString t_BoxSerial;

    static bool test;

    static QString t_version;
    static QString build_version;
    static QString temp_version;
    static QString ctrl_version;
    static bool bLogin;

    static int pubparam;

    static void CaliParamInit();
    static void SetCaliParam(const QString &caliName, int caliValue);
    static void SetTextParam(const QString &textName, QString textValue);
    static void LoadReagentBox();
    static void updateLockTime();
    static void updatetimezone();
    static void UISetDefaultParam();
    NetIpConnect *netipconnect;

public:
    void GlobalMessage(int code);
};

#endif // EXGLOBAL_H
