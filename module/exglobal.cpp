#include "sqlitemgr.h"
#include "exglobal.h"
#include "sequence.h"
#include "datahandler.h"

#include <QDebug>
#include <QCoreApplication>
#include "serialmgr.h"
#include <QNetworkInterface>
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include "systemcmd.h"

#include "dao/usbmodel.h"

#define BOXCODE_DEFAULT "302"

QString ExGlobal::t_panelCode = "012315";
QString ExGlobal::t_panelName = "上呼吸道测试";
QString ExGlobal::t_panelId = "80000";
QString ExGlobal::t_sampleCode = "";//"SLX 01079";
QString ExGlobal::t_sampleInfo = "";//"华山11";
QString ExGlobal::t_sampleRemark = "";
QString ExGlobal::t_BoxSerial = "Lot# 000001";
QString ExGlobal::User = "";
QString ExGlobal::DisplayUser = "";
int ExGlobal::UserType = 0;
QString ExGlobal::BoxCode = "";
bool ExGlobal::test = false;

QString ExGlobal::SysName = "全自动核酸检测分析系统";
QString ExGlobal::HospitalName = "";
QString ExGlobal::DataVersion = "V1.0";
int ExGlobal::PrintType = 0;

QString ExGlobal::AdminPassword = "123456";
int ExGlobal::LanguageCode = 0;
int ExGlobal::PanelBoxIndex = 1;

QString ExGlobal::t_version = "V1";
QString ExGlobal::build_version = "V1.0.3(build20201207)";
//QString ExGlobal::build_version = "V1.0.16(build20201012)";
QString ExGlobal::temp_version = "V0.00";
QString ExGlobal::ctrl_version = "V0.00";

bool ExGlobal::bLogin = false;

int ExGlobal::contentWidth = 1200;
int ExGlobal::contentHeight = 900;

int ExGlobal::V1WorkX = 9166;
int ExGlobal::V2WorkX = 8178;
int ExGlobal::V3WorkX = 5686;
int ExGlobal::VPWorkX = 9043;
int ExGlobal::V1SoftHomeX = 5890;
int ExGlobal::V2SoftHomeX = 4524;
int ExGlobal::V3SoftHomeX = 2410;
int ExGlobal::VPSoftHomeX = 5389;
int ExGlobal::VDWorkX = 2000;
int ExGlobal::VDSoftHomeX = 1000;

int ExGlobal::V1WorkOffset = 3276;
int ExGlobal::V2WorkOffset = 3276;
int ExGlobal::V3WorkOffset = 3276;
int ExGlobal::VPWorkOffset = 3654;
int ExGlobal::V1SoftHomeOffset = 2520;
int ExGlobal::V2SoftHomeOffset = 2520;
int ExGlobal::V3SoftHomeOffset = 2520;
int ExGlobal::VPSoftHomeOffset = 2520;
int ExGlobal::V1ToolHomeX = 9166;
int ExGlobal::V2ToolHomeX = 8178;
int ExGlobal::V3ToolHomeX = 5686;
int ExGlobal::VPToolHomeX = 9043;

int ExGlobal::CamAbs = 220;
int ExGlobal::CamGain = 10;
int ExGlobal::CamFocus = 0;
int ExGlobal::CamWhiteBlance = 0;

int ExGlobal::PumpSoftHomeX = 0;
int ExGlobal::PumpToolHomeX = 0;
int ExGlobal::PumpSoftHomeOffset = 460;

int ExGlobal::LockScreenTime = 0;
int ExGlobal::lockscreen_time = 0;
int ExGlobal::LockScreenOpen = 0;
int ExGlobal::TimeZone_s;
int ExGlobal::TimeZone = 0;
int ExGlobal::TimeZoneOpen = 1;
bool ExGlobal::bChildImage = false;
int ExGlobal::AutoFocus = 0;
int ExGlobal::QrCode = 0;
int ExGlobal::BlackLightPWM=255;


#if 0
int ExGlobal::QrX1 = 536;
int ExGlobal::QrY1 = 234;
int ExGlobal::QrX2 = 963;
int ExGlobal::QrY2 = 244;
int ExGlobal::QrX3 = 962;
int ExGlobal::QrY3 = 652;
int ExGlobal::QrX4 = 555;
int ExGlobal::QrY4 = 647;
#else
int ExGlobal::QrX1 = 424;
int ExGlobal::QrY1 = 204;
int ExGlobal::QrX2 = 952;
int ExGlobal::QrY2 = 217;
int ExGlobal::QrX3 = 975;
int ExGlobal::QrY3 = 731;
int ExGlobal::QrX4 = 456;
int ExGlobal::QrY4 = 715;
#endif
int ExGlobal::poxValue = 800;
int ExGlobal::VPump1 =6089;
int ExGlobal::VPump2 =5200;
int ExGlobal::VPump3 =4300;
int ExGlobal::VPump4 =0;
int ExGlobal::VPump5 =0;
int ExGlobal::VPump6 =0;
int ExGlobal::VPump7 =0;
int ExGlobal::VPump8 =0;
int ExGlobal::VPump9 =0;
int ExGlobal::VPump10 =0;
int ExGlobal::VPump11 =0;
int ExGlobal::VPump12 =0;
int ExGlobal::VPump13 =0;
int ExGlobal::VPump14 =0;
int ExGlobal::VPump15 =0;


uint16_t ExGlobal::PID_P1 =220;
uint16_t ExGlobal::PID_I1 =50;
uint16_t ExGlobal::PID_D1 =500;
uint16_t ExGlobal::PID_P2 =220;
uint16_t ExGlobal::PID_I2 =50;
uint16_t ExGlobal::PID_D2 =600;
uint16_t ExGlobal::PID_P3 =220;
uint16_t ExGlobal::PID_I3 =50;
uint16_t ExGlobal::PID_D3 =600;
uint16_t ExGlobal::PID_P4 =220;
uint16_t ExGlobal::PID_I4 =50;
uint16_t ExGlobal::PID_D4 =500;
uint16_t ExGlobal::PID_P5 =220;
uint16_t ExGlobal::PID_I5 =50;
uint16_t ExGlobal::PID_D5 =500;

uint16_t ExGlobal::PID_P6 =220;
uint16_t ExGlobal::PID_I6 =50;
uint16_t ExGlobal::PID_D6 =450;
uint16_t ExGlobal::PID_P7 =220;
uint16_t ExGlobal::PID_I7 =40;
uint16_t ExGlobal::PID_D7 =500;
uint16_t ExGlobal::PID_P8 =220;
uint16_t ExGlobal::PID_I8 =40;
uint16_t ExGlobal::PID_D8 =500;
uint16_t ExGlobal::PID_P9 =220;
uint16_t ExGlobal::PID_I9 =50;
uint16_t ExGlobal::PID_D9 =450;
uint16_t ExGlobal::PID_P10 =220;
uint16_t ExGlobal::PID_I10 =50;
uint16_t ExGlobal::PID_D10 =450;
uint16_t ExGlobal::PID_P11 =3000;
uint16_t ExGlobal::PID_I11 =100;
uint16_t ExGlobal::PID_D11 =2000;
uint16_t ExGlobal::PID_P12 =3000;
uint16_t ExGlobal::PID_I12 =100;
uint16_t ExGlobal::PID_D12 =2000;
uint16_t ExGlobal::PID_P13 =3000;
uint16_t ExGlobal::PID_I13 =100;
uint16_t ExGlobal::PID_D13 =2000;
uint16_t ExGlobal::PID_P14 =3000;
uint16_t ExGlobal::PID_I14 =100;
uint16_t ExGlobal::PID_D14 =2000;
uint16_t ExGlobal::PID_P15 =3000;
uint16_t ExGlobal::PID_I15 =100;
uint16_t ExGlobal::PID_D15 =2000;

int ExGlobal::ProjectMode = 1;
int ExGlobal::MachineMode = 1;
bool ExGlobal::HiddenFunction = false;
int ExGlobal::ClearMode = 2;
int ExGlobal::LiquidsHeight = 70;
int ExGlobal::DryWet = 125;
int ExGlobal::ClearWidth = 600;
int ExGlobal::FocusX = 400;
int ExGlobal::FocusY = 250;
int ExGlobal::FocusWidth = 1650;
int ExGlobal::FocusHeight = 1500;
int ExGlobal::LightCX = 1300;
int ExGlobal::LightCY = 1000;
int ExGlobal::LightR = 300;
int ExGlobal::DoorOut = 300;
int ExGlobal::DataEntry = 0;

int ExGlobal::pubparam = 0;
static uchar ReagentBox[121];
TestModel * ExGlobal::pTestModel = nullptr;
LogModel * ExGlobal::pLogModel = nullptr;
TestResultModel * ExGlobal::pTestResultModel = nullptr;
UserModel * ExGlobal::pUserModel = nullptr;
WifiModel * ExGlobal::pWifiModel = nullptr;
UsbModel * ExGlobal::pUsbModel = nullptr;

QMap<int, QString> ExGlobal::AssayItem;
QMap<int, QString> ExGlobal::ItemCode;
QHash<int, int> ExGlobal::ItemCT;
QMap<QString, QString> ExGlobal::ItemName;
QMap<QString, QString> ExGlobal::ItemFullName;

unsigned char * ExGlobal::bufrgb = nullptr;
unsigned char * ExGlobal::hbufrgb = nullptr;

QTranslator ExGlobal::tor;
QApplication * ExGlobal::app;

static ExGlobal *exGlobal = nullptr;

static SystemCmd cmd;

QString ExGlobal::DemoPanelCode = "20001";
QString ExGlobal::OnePointPanelCode = "31001";
int ExGlobal::SampleType = 0;

QDateTime ExGlobal::validDateTime = QDateTime::currentDateTime();
QList<int> ExGlobal::UIParam;

static QStringList SampleTypeArr;

ExGlobal *ExGlobal::getPtr()
{
    if (exGlobal == nullptr)
        exGlobal = new ExGlobal();
    return exGlobal;
}

ExGlobal::ExGlobal(QObject *parent) : QObject(parent)
{
    //qDebug()<<"ExGlobal";    
    UISetDefaultParam();
    netipconnect = nullptr;
}

void ExGlobal::GlobalMessage(int code){
    qDebug()<<"GlobalMessage:"<<code;
    exglobalMessage(code);
}

int ExGlobal::SysCommand(QString command){
    qDebug()<<"SysCommand"<<command;
    return system(command.toLatin1().data());
}

void ExGlobal::AsynCommand(QString command){
    cmd.execute(command);
}

void ExGlobal::set_backlight(float value)
{
   int pwm_value;
   pwm_value=(int)value;
   //qDebug()<<"set_backlight=="<<pwm_value;
   //system(QString("sudo echo %1 > /sys/class/backlight/backlight/brightness").arg(pwm_value).toLatin1());

   QFile file("/sys/class/backlight/backlight/brightness");
   if (!file.open(QIODevice::WriteOnly)){
       qDebug()<<"backlight file open error+++++++++++++++++";
       return;
   }
   QTextStream textStream(&file);

   textStream<<pwm_value<<endl;
   textStream.flush();
   file.close();

   updateCaliParam("BlackLightPWM",pwm_value);

}

float ExGlobal::read_backlight(void)
{
  return BlackLightPWM;

  QFile file("/sys/class/backlight/backlight/brightness");
  if (!file.open(QIODevice::ReadOnly))
  {
      qDebug()<<"Backlight file open failure!";
      return 255;
  }
  if (file.size()>0)
  {
      QTextStream in(&file);
      QString line=in.readLine();
      if (!line.isEmpty())
      {
        qDebug()<<"read_black_data:"<<line;
        return line.toFloat();
      }
   }
    return  255;
 }

void ExGlobal::exClose(){
    //cpu name: wmic cpu get Name
    qDebug()<<"exClose";
    //QProcess p;
    //p.start("halt");
    Log::Logdb(LOGTYPE_POWERDOWN);
    system("shutdown -t 0");
}

QString ExGlobal::getIP(){
    foreach(QNetworkInterface i,QNetworkInterface::allInterfaces()){
        if (i.name().contains("eth"))
        foreach(QNetworkAddressEntry e,i.addressEntries()){
            if (e.ip() != QHostAddress::LocalHost && e.ip().toIPv4Address())
                return e.ip().toString();
        }
    }
    /*
    foreach(const QHostAddress& hostAddress,QNetworkInterface::allAddresses())
        if(hostAddress != QHostAddress::LocalHost && hostAddress.toIPv4Address())
            return hostAddress.toString();
            */
    return "";
}

QString ExGlobal::getWIFIIP(){
    foreach(QNetworkInterface i,QNetworkInterface::allInterfaces()){
        if (i.name().contains("wlan"))
        foreach(QNetworkAddressEntry e,i.addressEntries()){
            if (e.ip() != QHostAddress::LocalHost && e.ip().toIPv4Address())
                return e.ip().toString();
        }
    }
    return "";
}

QString ExGlobal::getMac(){
    foreach(QNetworkInterface i,QNetworkInterface::allInterfaces()){
        if (i.name().contains("eth"))
            return i.hardwareAddress();
    }
    return "";
}

QString ExGlobal::getSerial(){
    QString serial =Log::readSerial();
    if (serial == "")
        return "000000000";
    return serial;
    //return "001001003";
    //return "001008003";
}

QStringList ExGlobal::getNetWork(){
    QStringList netArr;
    netArr<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<"";
    QFile file("/etc/network/interfaces.d/eth0");
    if (file.open(QIODevice::ReadOnly)){
        if (file.size()>0){
            QTextStream in(&file);
            QString line = in.readLine();
            while(!line.isEmpty())
            {
                qDebug()<<line;
                int addressIndex;

                if ((addressIndex = line.indexOf("address")) >= 0)
                {
                    QStringList strArr = line.mid(addressIndex+7).trimmed().split('.');
                    netArr[0] = strArr[0];
                    netArr[1] = strArr[1];
                    netArr[2] = strArr[2];
                    netArr[3] = strArr[3];
                }
                else if((addressIndex = line.indexOf("gateway")) >= 0)
                {
                    QStringList strArr = line.mid(addressIndex+7).trimmed().split('.');
                    netArr[4] = strArr[0];
                    netArr[5] = strArr[1];
                    netArr[6] = strArr[2];
                    netArr[7] = strArr[3];
                }
                else if((addressIndex = line.indexOf("netmask")) >= 0)
                {
                    QStringList strArr = line.mid(addressIndex+7).trimmed().split('.');
                    netArr[8] = strArr[0];
                    netArr[9] = strArr[1];
                    netArr[10] = strArr[2];
                    netArr[11] = strArr[3];
                }

                line = in.readLine();
            }
        }
        file.close();
    }
    foreach(const QHostAddress& hostAddress,QNetworkInterface::allAddresses())
        if(hostAddress != QHostAddress::LocalHost && hostAddress.toIPv4Address())
        {
            QStringList strArr = hostAddress.toString().split('.');
            netArr[0] = strArr[0];
            netArr[1] = strArr[1];
            netArr[2] = strArr[2];
            netArr[3] = strArr[3];
            break;
        }
    return netArr;
}

void ExGlobal::setNetWork(const QString &ip, const QString &mask, const QString &gate){
    QFile file("/etc/network/interfaces.d/eth0");
    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream text(&file);
        text<<"auto eth0"<<endl<<"iface eth0 inet static"<<endl;
        text<<"address "<<ip<<endl;
        text<<"gateway "<<gate<<endl;
        text<<"netmask "<<mask<<endl;
        text<<"dns-nameservers "<<gate<<endl;
        text.flush();
        file.close();

        if (getIP() != ip)
		{
        //    system("sudo /etc/init.d/networking restart");
         system("sudo ifdown -a");
         system("sudo ifup -a");
		 }
    }
    qDebug()<<"setNetWork"<<ip<<mask<<gate;
}
NetIpConnect::NetIpConnect(QObject *parent) : QObject(parent)
{
    qDebug()<<"new NetIpConnect";
    mode = 0;
}
void NetIpConnect::start()
{
    QFile file("/etc/network/interfaces.d/eth0");
    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream text(&file);
        text<<"auto eth0"<<endl<<"iface eth0 inet static"<<endl;
        text<<"address "<<ip<<endl;
        text<<"gateway "<<gate<<endl;
        text<<"netmask "<<mask<<endl;
        text<<"dns-nameservers "<<gate<<endl;
        text.flush();
        file.close();

       // if (ExGlobal::getIP() != ip)
       // {
        //    system("sudo /etc/init.d/networking restart");
         system("sudo ifdown -a");
         system("sudo ifup -a");
       //  }
        emit Progress(1);
    }
    qDebug()<<"setNetWork"<<ip<<mask<<gate;


}

void ExGlobal::ConnectFinish(int result){
    netipconnect = nullptr;
  //refresh();
  //  qDebug()<<"wifi_connect_result_pre:"<<result;
   emit netFinish(result);
}

void ExGlobal::setNetIp(const QString &ip, const QString &mask, const QString &gate){

    if (netipconnect != nullptr)
        return;
    netipconnect = new NetIpConnect;
    QObject::connect(netipconnect,SIGNAL(Progress(int)),this,SLOT(ConnectFinish(int)));
    QThread *thread = new QThread;
    netipconnect->moveToThread(thread);
    QObject::connect(thread,SIGNAL(started()),netipconnect,SLOT(start()));
    QObject::connect(netipconnect,SIGNAL(Finish()),netipconnect,SLOT(deleteLater()));
    QObject::connect(netipconnect,SIGNAL(destroyed()),thread,SLOT(quit()));
    QObject::connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    netipconnect->setParam(ip,mask,gate);
   // connector->setMode(0);
  // ssid_ok_name=ssid;
   // password_ok=password;
  //  qDebug()<<"wifi_name:"<<ssid<<"++++++++++++++++++++++++wifi_password:"<<password;
    thread->start();
}

static int hwSetTimer(){
    int result = system("hwclock --systohc");
    qDebug()<<"HwsetTime,result OK="<<result;
    return result;
}

bool ExGlobal::setTime(QString time){
    QString order = "date -s '"+time+"'";
    int result = system(order.toLatin1().data());
    qDebug()<<"setTime,result="<<result;
    MethodToThread(hwSetTimer);
    return true;
}

static int AvailableDisk = 0;
int ExGlobal::diskCheck(){     
    if (system("df / > /opt/diskspacet") == 0){
        QFile file("/opt/diskspacet");
        if (!file.open(QIODevice::ReadOnly)){
            qDebug()<<"getDiskSpace open file error!";
            return 0;
        }
        if (file.size()>0){
            QTextStream in(&file);
            QString line = in.readLine();
            if (!line.isEmpty()){
                int availableIndex = line.indexOf("Available");
                int usepaIndex = line.indexOf("Use%");
                line = in.readLine();
                if (!line.isEmpty())
                {
                    QString available = line.mid(availableIndex,(usepaIndex-availableIndex)).simplified();
                    qDebug()<<"diskCheck"<<available;
                    AvailableDisk = available.toInt();
                }
            }
        }
        file.close();
    }    

    if (AvailableDisk < 1000000)
        return 1;
    else if (AvailableDisk < 5000000)
        return 2;
    else if (AvailableDisk < 10000000)
        return 3;

    return 0;
}

static bool CopyDir(const QString &fromDir, const QString &toDir, bool cover){
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if (!targetDir.exists()){
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }
    sourceDir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = sourceDir.entryInfoList();
    foreach(QFileInfo file, fileList){
        if (file.isFile()){
            //qDebug()<<"copy file"<<file.absoluteFilePath();
            if (cover && targetDir.exists(file.fileName()))
                targetDir.remove(file.fileName());
            if (!QFile::copy(file.filePath(),targetDir.filePath(file.fileName())))
                return false;
        }
        else{
            if (!CopyDir(file.filePath(),targetDir.filePath(file.fileName()),cover))
                return false;
        }
    }
    return true;
}

static bool DelDir(const QString &path){
    qDebug()<<"DelDir"<<path;
    if (path.isEmpty())
        return false;
    QDir dir(path);
    if (!dir.exists())
        return true;
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach(QFileInfo file, fileList){
        if (file.isFile())
            file.dir().remove(file.fileName());
        else
            DelDir(file.absoluteFilePath());
    }
    return dir.rmpath(dir.absolutePath());
}

static int DiskSpaceCheck(){
    int result = ExGlobal::diskCheck();
    int trycount = 0;
    int delcount = 0;
    QDir dirbk(QCoreApplication::applicationDirPath()+"/bk");
    if (!dirbk.exists())
        dirbk.mkdir(QCoreApplication::applicationDirPath()+"/bk");

    while (result != 0 && trycount++ < 10){
        QDir dir(QCoreApplication::applicationDirPath());
        QStringList filters;
        filters<<"20*";
        QStringList dirList = dir.entryList(filters,QDir::Dirs,QDir::Name);
        foreach(QString node, dirList){
            qDebug()<<node;
            QDir dirLinuxData(QCoreApplication::applicationDirPath()+"/"+node+"/LinuxData");
            if (dirLinuxData.exists()){
                QString CopyTo = QCoreApplication::applicationDirPath() + "/bk/"+node;
                QDir dirTo(CopyTo);
                if (!dirTo.exists())
                    dirTo.mkdir(CopyTo);
                CopyDir(QCoreApplication::applicationDirPath()+"/"+node+"/LinuxData",CopyTo,false);
            }
            DelDir(QCoreApplication::applicationDirPath()+"/"+node);
            if (++delcount >= 10)
            {
                delcount = 0;
                break;
            }
        }        
        result = ExGlobal::diskCheck();
        qDebug()<<"DiskSpaceCheck"<<result<<trycount;
    }

    return result;
}

void ExGlobal::DiskCheckFinish(int result){
    qDebug()<<"DiskCheckFinish"<<result<<AvailableDisk;
}

QString ExGlobal::diskSpac(){
    return QString("%1M").arg(AvailableDisk/1024);
}

void ExGlobal::exInit()
{
    ExGlobal::pTestModel = new TestModel();
    ExGlobal::pLogModel = new LogModel();
    ExGlobal::pTestResultModel = new TestResultModel();
    ExGlobal::pUserModel = new UserModel();
    ExGlobal::pWifiModel = new WifiModel();
    ExGlobal::pUsbModel = new UsbModel();

    CaliParamInit();
    SerialMgr::SerialInit();    
    DataHandler::setAssayItem(AssayItem);    
    DataHandler::setItemName(ItemName);
    DataHandler::setItemCT(ItemCT);
    //Sequence::sequenceInit();
    MethodToThread(DiskSpaceCheck,exGlobal,SLOT(DiskCheckFinish(int)));
}

void ExGlobal::CaliParamInit()
{
    QString sql = "SELECT * FROM CaliParam";
    QSqlQuery query = SqliteMgr::select(sql);

    while(query.next())
    {
        SetCaliParam(query.value(0).toString(),query.value(1).toInt());
    }

    sql = "select * from TextParam";
    query = SqliteMgr::select(sql);
    while(query.next()){
        SetTextParam(query.value(0).toString(),query.value(1).toString());
    }

    sql = "select * from AssayItem order by Itemid";
    query = SqliteMgr::select(sql);
    while(query.next()){
        //qDebug()<<"Itemid:"<<query.value(0).toInt()<<"ItemName:"<<query.value(1).toString()<<"ItemCT:"<<query.value(2).toInt();
        AssayItem[query.value(0).toInt()] = query.value(1).toString();
        ItemCode[query.value(0).toInt()] = query.value(4).toString();
        ItemCT[query.value(0).toInt()] = query.value(2).toInt();
        ItemName[query.value(1).toString()] = query.value(4).toString();
    }

    pUserModel->LoadUser();
    pWifiModel->LoadData();
    updateLockTime();

    Log::Logdb(LOGTYPE_POWERON);
    qDebug()<<"CaliParamInit end";
    set_backlight(BlackLightPWM);
    Translator(LanguageCode);
}

uchar* ExGlobal::getReagentBox(QString boxCode){
    if (boxCode == "")
        boxCode = BOXCODE_DEFAULT;
    if (boxCode != BoxCode)
    {
        BoxCode = boxCode;
        LoadReagentBox();
    }
    return ReagentBox;
}

void ExGlobal::SetCaliParam(const QString &name, int caliValue)
{
    if (name == "V1WorkX")
        V1WorkX = caliValue;
    else if (name == "V2WorkX")
        V2WorkX = caliValue;
    else if (name == "V2WorkX")
        V2WorkX = caliValue;
    else if (name == "V3WorkX")
        V3WorkX = caliValue;
    else if (name == "VPWorkX")
        VPWorkX = caliValue;
    else if (name == "V1SoftHomeX")
        V1SoftHomeX = caliValue;
    else if (name == "V2SoftHomeX")
        V2SoftHomeX = caliValue;
    else if (name == "V3SoftHomeX")
        V3SoftHomeX = caliValue;
    else if (name == "VPSoftHomeX")
        VPSoftHomeX = caliValue;
    else if (name == "V1SoftHomeOffset")
        V1SoftHomeOffset = caliValue;
    else if (name == "V2SoftHomeOffset")
        V2SoftHomeOffset = caliValue;
    else if (name == "V3SoftHomeOffset")
        V3SoftHomeOffset = caliValue;
    else if (name == "VPSoftHomeOffset")
        VPSoftHomeOffset = caliValue;
    else if (name == "V1ToolHomeX")
        V1ToolHomeX = caliValue;
    else if (name == "V2ToolHomeX")
        V2ToolHomeX = caliValue;
    else if (name == "V3ToolHomeX")
        V3ToolHomeX = caliValue;
    else if (name == "VPToolHomeX")
        VPToolHomeX = caliValue;
    else if (name == "V1WorkOffset")
        V1WorkOffset = caliValue;
    else if (name == "V2WorkOffset")
        V2WorkOffset = caliValue;
    else if (name == "V3WorkOffset")
        V3WorkOffset = caliValue;
    else if (name == "VPWorkOffset")
        VPWorkOffset = caliValue;
    else if (name == "VDWorkX")
        VDWorkX = caliValue;
    else if (name == "VDSoftHomeX")
        VDSoftHomeX = caliValue;
    else if (name == "V2WorkX")
        V2WorkX = caliValue;
    else if (name == "PumpSoftHomeX")
        PumpSoftHomeX = caliValue;
    else if (name == "PumpToolHomeX")
        PumpToolHomeX = caliValue;
    else if (name == "PumpSoftHomeOffset")
        PumpSoftHomeOffset = caliValue;

    if (name == "CamAbs")
        CamAbs = caliValue;
    else if (name == "CamGain")
        CamGain = caliValue;
    else if (name == "CamFocus")
        CamFocus = caliValue;
    else if (name == "CamWhiteBlance")
        CamWhiteBlance = caliValue;
    else if (name == "LockScreenTime")
        LockScreenTime = caliValue;
    else if (name == "LockScreenOpen")
        LockScreenOpen = caliValue;
    else if (name == "TimeZone")
        TimeZone = caliValue;
    else if (name == "TimeZoneOpen")
        TimeZoneOpen = caliValue;
    else if (name == "BlackLightPWM")
        BlackLightPWM = caliValue;
    else if (name == "LanguageCode")
        LanguageCode = caliValue;
    else if (name == "PanelBoxIndex")
        PanelBoxIndex = caliValue;
    else if (name == "AutoFocus")
        AutoFocus = caliValue;
    else if (name == "QrCode")
        QrCode = caliValue;
    else if(name == "QrX1")
        QrX1 = caliValue;
    else if(name == "QrY1")
        QrY1 = caliValue;
    else if(name == "QrX2")
        QrX2 = caliValue;
    else if(name == "QrY2")
        QrY2 = caliValue;
    else if(name == "QrX3")
        QrX3 = caliValue;
    else if(name == "QrY3")
        QrY3 = caliValue;
    else if(name == "QrX4")
        QrX4 = caliValue;
    else if(name == "QrY4")
        QrY4 = caliValue;
    else if(name == "poxValue")
        poxValue = caliValue;
    else if(name == "ProjectMode")
        ProjectMode = caliValue;
    else if(name == "MachineMode")
        MachineMode = caliValue;
    else if(name == "ClearMode")
        ClearMode = caliValue;
    else if(name == "ClearWidth")
        ClearWidth = caliValue;
    else if(name == "FocusX")
        FocusX = caliValue;
    else if(name == "FocusY")
        FocusY = caliValue;
    else if(name == "FocusWidth")
        FocusWidth = caliValue;
    else if(name == "FocusHeight")
        FocusHeight = caliValue;
    else if(name == "LightCX")
        LightCX = caliValue;
    else if(name == "LightCY")
        LightCY = caliValue;
    else if(name == "LightR")
        LightR = caliValue;
    else if(name == "DryWet")
        DryWet = caliValue;
    else if(name == "LiquidsHeight")
        LiquidsHeight = caliValue;
    else if(name == "PrintType")
        PrintType = caliValue;
    else if(name == "VPump1")
        VPump1 = caliValue;
    else if(name == "VPump2")
        VPump2 = caliValue;
    else if(name == "VPump3")
        VPump3 = caliValue;
    else if(name == "VPump4")
        VPump4 = caliValue;
    else if(name == "VPump5")
        VPump5 = caliValue;
    else if(name == "VPump6")
        VPump6 = caliValue;
    else if(name == "VPump7")
        VPump7 = caliValue;
    else if(name == "VPump8")
        VPump8 = caliValue;
    else if(name == "VPump9")
        VPump9 = caliValue;
    else if(name == "VPump10")
        VPump10 = caliValue;
    else if(name == "VPump11")
        VPump11 = caliValue;
    else if(name == "VPump12")
        VPump12 = caliValue;
    else if(name == "VPump13")
        VPump13 = caliValue;
    else if(name == "VPump14")
        VPump14 = caliValue;
    else if(name == "VPump15")
        VPump15 = caliValue;
    else if(name == "PID_P1")
        PID_P1 = caliValue;
    else if(name == "PID_I1")
        PID_I1 = caliValue;
    else if(name == "PID_D1")
        PID_D1 = caliValue;
    else if(name == "PID_P2")
        PID_P2 = caliValue;
    else if(name == "PID_I2")
        PID_I2 = caliValue;
    else if(name == "PID_D2")
        PID_D2 = caliValue;
    else if(name == "PID_P3")
        PID_P3 = caliValue;
    else if(name == "PID_I3")
        PID_I3 = caliValue;
    else if(name == "PID_D3")
        PID_D3 = caliValue;
    else if(name == "PID_P4")
        PID_P4 = caliValue;
    else if(name == "PID_I4")
        PID_I4 = caliValue;
    else if(name == "PID_D4")
        PID_D4 = caliValue;
    else if(name == "PID_P5")
        PID_P5 = caliValue;
    else if(name == "PID_I5")
        PID_I5 = caliValue;
    else if(name == "PID_D5")
        PID_D5 = caliValue;
    else if(name == "PID_P6")
        PID_P6 = caliValue;
    else if(name == "PID_I6")
        PID_I6 = caliValue;
    else if(name == "PID_D6")
        PID_D6 = caliValue;
    else if(name == "PID_P7")
        PID_P7 = caliValue;
    else if(name == "PID_I7")
        PID_I7 = caliValue;
    else if(name == "PID_D7")
        PID_D7 = caliValue;
    else if(name == "PID_P8")
        PID_P8 = caliValue;
    else if(name == "PID_I8")
        PID_I8 = caliValue;
    else if(name == "PID_D8")
        PID_D8 = caliValue;
    else if(name == "PID_P9")
        PID_P9 = caliValue;
    else if(name == "PID_I9")
        PID_I9 = caliValue;
    else if(name == "PID_D9")
        PID_D9 = caliValue;
    else if(name == "PID_P10")
        PID_P10 = caliValue;
    else if(name == "PID_I10")
        PID_I10 = caliValue;
    else if(name == "PID_D10")
        PID_D10 = caliValue;
    else if(name == "PID_P11")
        PID_P11 = caliValue;
    else if(name == "PID_I11")
        PID_I11 = caliValue;
    else if(name == "PID_D11")
        PID_D11 = caliValue;
    else if(name == "PID_P12")
        PID_P12 = caliValue;
    else if(name == "PID_I12")
        PID_I12 = caliValue;
    else if(name == "PID_D12")
        PID_D12 = caliValue;
    else if(name == "PID_P13")
        PID_P13 = caliValue;
    else if(name == "PID_I13")
        PID_I13 = caliValue;
    else if(name == "PID_D13")
        PID_D13 = caliValue;
    else if(name == "PID_P14")
        PID_P14 = caliValue;
    else if(name == "PID_I14")
        PID_I14 = caliValue;
    else if(name == "PID_D14")
        PID_D14 = caliValue;
    else if(name == "PID_P15")
        PID_P15 = caliValue;
    else if(name == "PID_I15")
        PID_I15 = caliValue;
    else if(name == "PID_D15")
        PID_D15 = caliValue;

    qDebug()<<"setCaliParam,"<<name<<",result="<<caliValue;
}

int ExGlobal::getPumpParam(int PumpName)
{
    if  (PumpName==1)
      return VPump1;
    else if (PumpName==2)
      return VPump2;
    else if (PumpName==3)
      return VPump3;
    else if (PumpName==4)
      return VPump4;
    else if (PumpName==5)
      return VPump5;
    else if (PumpName==6)
      return VPump6;
    else if (PumpName==7)
      return VPump7;
    else if (PumpName==8)
      return VPump8;
    else if (PumpName==9)
      return VPump9;
    else if (PumpName==10)
      return VPump10;
    else if (PumpName==11)
      return VPump11;
    else if (PumpName==12)
      return VPump12;
    else if (PumpName==13)
      return VPump13;
    else if (PumpName==14)
      return VPump14;
    else if (PumpName==15)
      return VPump15;
    return VPump1;
}

int ExGlobal::getLJTemp(int param)
 {
    if  (param==1)
      return VPump11;
    else if (param==2)
      return VPump12;
    else if (param==3)
      return VPump13;
    else if (param==4)
      return VPump14;
    else if (param==5)
      return VPump15;


}
int ExGlobal::getCaliParam(const QString &caliName)
{
    int result = 0;
    if (caliName == "V1WorkX")
        result = V1WorkX;
    else if(caliName == "V2WorkX")
        result = V2WorkX;
    else if(caliName == "V3WorkX")
        result = V3WorkX;
    else if(caliName == "VPWorkX")
        result = VPWorkX;
    else if(caliName == "V1ToolHomeX")
        result = V1ToolHomeX;
    else if(caliName == "V2ToolHomeX")
        result = V2ToolHomeX;
    else if(caliName == "V3ToolHomeX")
        result = V3ToolHomeX;
    else if(caliName == "VPToolHomeX")
        result = VPToolHomeX;
    else if(caliName == "V1SoftHomeX")
        result = V1SoftHomeX;
    else if(caliName == "V2SoftHomeX")
        result = V2SoftHomeX;
    else if(caliName == "V3SoftHomeX")
        result = V3SoftHomeX;
    else if(caliName == "VPSoftHomeX")
        result = VPSoftHomeX;
    else if(caliName == "V1SoftHomeOffset")
        result = V1SoftHomeOffset;
    else if(caliName == "V2SoftHomeOffset")
        result = V2SoftHomeOffset;
    else if(caliName == "V3SoftHomeOffset")
        result = V3SoftHomeOffset;
    else if(caliName == "VPSoftHomeOffset")
        result = VPSoftHomeOffset;
    else if(caliName == "V1WorkOffset")
        result = V1WorkOffset;
    else if(caliName == "V2WorkOffset")
        result = V2WorkOffset;
    else if(caliName == "V3WorkOffset")
        result = V3WorkOffset;
    else if(caliName == "VPWorkOffset")
        result = VPWorkOffset;
    else if(caliName == "VDWorkX")
        result = VDWorkX;
    else if(caliName == "VDSoftHomeX")
        result = VDSoftHomeX;
    else if(caliName == "PumpSoftHomeX")
        result = PumpSoftHomeX;
    else if(caliName == "PumpToolHomeX")
        result = PumpToolHomeX;
    else if(caliName == "PumpSoftHomeOffset")
        result = PumpSoftHomeOffset;

    if (caliName == "CamAbs")
        result = CamAbs;
    else if(caliName == "CamGain")
        result = CamGain;
    else if(caliName == "CamFocus")
        result = CamFocus;
    else if(caliName == "CamWhiteBlance")
        result = CamWhiteBlance;
    else if(caliName == "LockScreenTime")
        result = LockScreenTime;
    else if(caliName == "LockScreenOpen")
        result = LockScreenOpen;
    else if(caliName == "TimeZone")
        result = TimeZone;
    else if(caliName == "TimeZoneOpen")
        result = TimeZoneOpen;
    else if(caliName == "BlackLightPWM")
        result = BlackLightPWM;
    else if(caliName == "LanguageCode")
        result = LanguageCode;
    else if(caliName == "PanelBoxIndex")
        result = PanelBoxIndex;
    else if(caliName == "AutoFocus")
        result = AutoFocus;
    else if(caliName == "QrCode")
        result = QrCode;
    else if(caliName == "QrX1")
        result = QrX1;
    else if(caliName == "QrY1")
        result = QrY1;
    else if(caliName == "QrX2")
        result = QrX2;
    else if(caliName == "QrY2")
        result = QrY2;
    else if(caliName == "QrX3")
        result = QrX3;
    else if(caliName == "QrY3")
        result = QrY3;
    else if(caliName == "QrX4")
        result = QrX4;
    else if(caliName == "QrY4")
        result = QrY4;
    else if(caliName == "poxValue")
        result = poxValue;
    else if(caliName == "ProjectMode")
        result = ProjectMode;
    else if(caliName == "MachineMode")
        result = MachineMode;
    else if(caliName == "ClearMode")
        result = ClearMode;
    else if(caliName == "ClearWidth")
        result = ClearWidth;
    else if(caliName == "FocusX")
        result = FocusX;
    else if(caliName == "FocusY")
        result = FocusY;
    else if(caliName == "FocusWidth")
        result = FocusWidth;
    else if(caliName == "FocusHeight")
        result = FocusHeight;
    else if(caliName == "LightCX")
        result = LightCX;
    else if(caliName == "LightCY")
        result = LightCY;
    else if(caliName == "LightR")
        result = LightR;
    else if(caliName == "DryWet")
        result = DryWet;
    else if(caliName == "LiquidsHeight")
        result = LiquidsHeight;
    else if(caliName == "PrintType")
        result = PrintType;
    else if(caliName == "VPump1")
        result =  VPump1;
    else if(caliName == "VPump2")
        result =  VPump2;
    else if(caliName == "VPump3")
        result =  VPump3;
    else if(caliName == "VPump4")
        result =  VPump4;
    else if(caliName == "VPump5")
        result =  VPump5;
    else if(caliName == "VPump6")
        result =  VPump6;
    else if(caliName == "VPump7")
        result =  VPump7;
    else if(caliName == "VPump8")
        result =  VPump8;
    else if(caliName == "VPump9")
        result =  VPump9;
    else if(caliName == "VPump10")
        result =  VPump10;
    else if(caliName == "VPump11")
        result =  VPump11;
    else if(caliName == "VPump12")
        result =  VPump12;
    else if(caliName == "VPump13")
        result =  VPump13;
    else if(caliName == "VPump14")
        result =  VPump14;
    else if(caliName == "VPump15")
        result =  VPump15;
    else if(caliName == "PID_P1")
        result =  PID_P1;
    else if(caliName == "PID_I1")
        result =  PID_I1;
    else if(caliName == "PID_D1")
        result =  PID_D1;
    else if(caliName == "PID_P2")
        result =  PID_P2;
    else if(caliName == "PID_I2")
        result =  PID_I2;
    else if(caliName == "PID_D2")
        result =  PID_D2;
    else if(caliName == "PID_P3")
        result =  PID_P3;
    else if(caliName == "PID_I3")
        result =  PID_I3;
    else if(caliName == "PID_D3")
        result =  PID_D3;
    else if(caliName == "PID_P4")
        result =  PID_P4;
    else if(caliName == "PID_I4")
        result =  PID_I4;
    else if(caliName == "PID_D4")
        result =  PID_D4;
    else if(caliName == "PID_P5")
        result =  PID_P5;
    else if(caliName == "PID_I5")
        result =  PID_I5;
    else if(caliName == "PID_D5")
        result =  PID_D5;

    else if(caliName == "PID_P6")
        result =  PID_P6;
    else if(caliName == "PID_I6")
        result =  PID_I6;
    else if(caliName == "PID_D6")
        result =  PID_D6;
    else if(caliName == "PID_P7")
        result =  PID_P7;
    else if(caliName == "PID_I7")
        result =  PID_I7;
    else if(caliName == "PID_D7")
        result =  PID_D7;
    else if(caliName == "PID_P8")
        result =  PID_P8;
    else if(caliName == "PID_I8")
        result =  PID_I8;
    else if(caliName == "PID_D8")
        result =  PID_D8;
    else if(caliName == "PID_P9")
        result =  PID_P9;
    else if(caliName == "PID_I9")
        result =  PID_I9;
    else if(caliName == "PID_D9")
        result =  PID_D9;
    else if(caliName == "PID_P10")
        result =  PID_P10;
    else if(caliName == "PID_I10")
        result =  PID_I10;
    else if(caliName == "PID_D10")
        result =  PID_D10;

    else if(caliName == "PID_P11")
        result =  PID_P11;
    else if(caliName == "PID_I11")
        result =  PID_I11;
    else if(caliName == "PID_D11")
        result =  PID_D11;
    else if(caliName == "PID_P12")
        result =  PID_P12;
    else if(caliName == "PID_I12")
        result =  PID_I12;
    else if(caliName == "PID_D12")
        result =  PID_D12;
    else if(caliName == "PID_P13")
        result =  PID_P13;
    else if(caliName == "PID_I13")
        result =  PID_I13;
    else if(caliName == "PID_D13")
        result =  PID_D13;
    else if(caliName == "PID_P14")
        result =  PID_P14;
    else if(caliName == "PID_I14")
        result =  PID_I14;
    else if(caliName == "PID_D14")
        result =  PID_D14;
    else if(caliName == "PID_P15")
        result =  PID_P15;
    else if(caliName == "PID_I15")
        result =  PID_I15;
    else if(caliName == "PID_D15")
        result =  PID_D15;

    qDebug()<<"getCaliParam,"<<caliName<<",result="<<result;
    return result;
}

void ExGlobal::updateCaliParam(const QString &caliName, int caliValue)
{
    QString sql = "SELECT * FROM CaliParam WHERE ParamName = '"+caliName+"'";
    QSqlQuery query = SqliteMgr::select(sql);
    if (query.next())
        sql = "UPDATE CaliParam SET ParamValue = "+ QString::number(caliValue)+" WHERE ParamName = '"+ caliName +"'";
    else
        sql = "INSERT INTO CaliParam (ParamName, ParamValue) VALUES ('"+caliName+"', "+ QString::number(caliValue)+")";
    SqliteMgr::execute(sql);
    qDebug()<<"updateCaliParam,"<<caliName<<",result="<<caliValue;
    SetCaliParam(caliName,caliValue);
}

void ExGlobal::SetTextParam(const QString &textName, QString textValue){
    if (textName == "SysName")
        SysName = textValue;
    else if(textName == "AdminPassword")
        AdminPassword = textValue;
    else if(textName == "HospitalName")
        HospitalName = textValue;
    else if(textName == "Version")
        DataVersion = textValue;

}

QString ExGlobal::getTextParam(const QString &caliName){
    QString result;
    if (caliName == "SysName")
        result = SysName;
    else if(caliName == "AdminPassword")
        result = AdminPassword;
    else if(caliName == "HospitalName")
        result = HospitalName;
    else if(caliName == "Version")
        result = DataVersion;
    return result;
}

void ExGlobal::updateTextParam(const QString &textName, QString textValue){
    QString sql = QString("replace into TextParam(ParamName,ParamValue) values('%1','%2')").arg(textName).arg(textValue);
    SqliteMgr::execute(sql);
    SetTextParam(textName,textValue);
}

QString ExGlobal::sysLanguageName()
{
    QString lang = "中文";
    if (LanguageCode == 1)
        lang = "English";

    return lang;
}
QStringList ExGlobal::serialPort(){
    QStringList port;
    port<<"ttyUSB0"<<"ttyS0"<<"ttyS4";
    return port;
}

QString ExGlobal::getPosName(int itemid){
    return ItemName[AssayItem[itemid]];
}

QString ExGlobal::getPosFullName(int itemid){
    return ItemFullName[AssayItem[itemid]];
}

QString ExGlobal::getItemCode(int itemid){
    return ItemCode[itemid];
}

QString ExGlobal::ItemCodeToFullName(QString itemCode){
    for(QMap<int, QString>::iterator it = ItemCode.begin(); it != ItemCode.end(); it++ ){
        if (it.value() == itemCode){
            return ItemFullName[AssayItem[it.key()]];
        }
    }
    return "";
}

int ExGlobal::getItemCT(int Itemid){
    //qDebug()<<"getItemCT,Itemid="<<Itemid<<",CT="<<ItemCT[Itemid];
    //return 10;
    return ItemCT[Itemid];
}

QList<int> ExGlobal::getBoxItemList(){
    QList<int> result;
    qDebug()<<"getBoxItemList"<<pTestModel->getCurrTestBoxCode();
    QString sql = "SELECT * FROM ReagentPos where BoxCode='"+pTestModel->getCurrTestBoxCode()+"'";
    QSqlQuery query = SqliteMgr::select(sql);
    while(query.next()){
        bool find = false;
        int value = query.value(2).toInt();
        if (value > 2){
            for (int j = 0; j < result.size(); j++)
                if (result[j] == value)
                {
                    find = true;
                    break;
                }
            if (find == false)
                result<<value;
        }
    }

    sort(result.begin(),result.end());
    qDebug()<<result;
    return result;
}

QList<int> ExGlobal::getBoxItemListByName(QString name){
    QList<int> result;
    QMap<QString,QString>::iterator it;
    for(it = ItemName.begin(); it != ItemName.end(); it++ ){
        if (it.value() == name){            
            for (QMap<int,QString>::iterator itt = AssayItem.begin();itt!=AssayItem.end();itt++){
                if (itt.value() == it.key()){
                    result<<itt.key();
                    break;
                }
            }
        }
    }
    return result;
}

QStringList ExGlobal::getBoxItemName(){
    QStringList result;
    QString sql = "SELECT * FROM ReagentPos where BoxCode='"+pTestModel->getCurrTestBoxCode()+"'";
    if (pTestModel->getCurrTestBoxCode() == "205")
        sql = "SELECT * FROM ReagentPos where BoxCode='204'";
    QSqlQuery query = SqliteMgr::select(sql);
    while(query.next()){
        bool find = false;
        int value = query.value(2).toInt();
        if (value > 2){
            QString name = ItemName[AssayItem[value]];
            for (int j = 0; j < result.size(); j++)
                if (result[j] == name)
                {
                    find = true;
                    break;
                }
            if (find == false)
                result<<name;
        }
    }

    //sort(result.begin(),result.end());
    //qDebug()<<result;
    //QStringList resultt;
    //resultt<<"甲型流感病毒"<<"禽流感H5N1"<<"甲流H1N1-pdm"<<"乙型流感病毒"<<"呼吸道合胞病毒A"<<"呼吸道合胞病毒B"<<"副流感病毒1型"<<"副流感病毒2型"<<"副流感病毒3型";
    //resultt<<"腺病毒C型"<<"腺病毒B/E型"<<"肺炎支原体"<<"百日咳杆菌"<<"新型冠状病毒";
    //resultt<<"FluA"<<"FluA-H5N1"<<"FluA-H1N1-2009"<<"FluB"<<"RSV-A"<<"RSV-B"<<"PIV-1"<<"PIV-2"<<"PIV-3"<<"ADV-C"<<"ADV-B/E"<<"MP"<<"BP"<<"SARS-CoV-2";
    return result;
}

QStringList ExGlobal::getPosNameArray(){
    QStringList nameArr;
    getReagentBox(reagentBox());
    for (int i = 0; i < 121; i++)
        if (ReagentBox[i] > 1){
            QString name = AssayItem[ReagentBox[i]];
            bool findit = false;
            for (int i = 0; i < nameArr.size(); i++){
                if(nameArr[i] == name){
                    findit = true;
                    break;
                }
            }
            if (findit == false)
                nameArr<<name;
        }

    return nameArr;
}

void ExGlobal::panelBoxIndexAddOne(){
    updateCaliParam("PanelBoxIndex",PanelBoxIndex + 1);
    t_BoxSerial = QString("Lot# %1").arg(PanelBoxIndex, 6, 10, QChar('0'));
    //t_sampleCode = QString("SLX %1").arg(PanelBoxIndex, 6, 10, QChar('0'));
}

QString ExGlobal::reagentBox(){
    if (BoxCode == "")
    {
        BoxCode = BOXCODE_DEFAULT;
        LoadReagentBox();
    }
    return BoxCode;
}

void ExGlobal::setReagentBox(const QString &reagentBoxType){
    QString BoxType = reagentBoxType;
    //if (BoxType == "302")
    //    BoxType = "303";
    qDebug()<<"setReagentBox"<<BoxType<<BoxCode;
    if (BoxType != BoxCode){
        BoxCode = BoxType;
        LoadReagentBox();
    }
}

void ExGlobal::LoadReagentBox(){
    QString sql = "SELECT * FROM ReagentPos where BoxCode='"+BoxCode+"'";
    QSqlQuery query = SqliteMgr::select(sql);
    memset(ReagentBox,0,sizeof(ReagentBox));
    while(query.next()){
        if (query.value(1).toInt() < 121)
            ReagentBox[query.value(1).toInt()] = static_cast<uchar>(query.value(2).toInt());
    }
}

void ExGlobal::setSysLanguageCode(int code){
    if (code != LanguageCode)
    {
        //QApplication::installTranslator(&tor);
        //QApplication::removeTranslator(&tor);
        qDebug()<<"setLanguage"<<code;
        if (code == 0)
        {
            app->removeTranslator(&tor);
            setUIParam(0);
            QSqlQuery query = SqliteMgr::select("select * from AssayItem order by Itemid");
            while(query.next()){
                ItemName[query.value(1).toString()] = query.value(3).toString();
                ItemFullName[query.value(1).toString()] = query.value(6).toString();
            }

            SampleTypeArr.clear();
            SampleTypeArr<<tr("鼻咽拭子")<<tr("咽拭子");
        }
        else
            Translator(code);

        //qml->retranslate();
        updateCaliParam("LanguageCode",code);
        emit sysLanguageCodeChanged();
        emit uiParamChanged();
    }
}

void ExGlobal::Translator(int language){
    if (language == 1)
    {
        tor.load(QCoreApplication::applicationDirPath()+"/en_US.qm");
        app->installTranslator(&tor);
    }
    setUIParam(language);    

    QSqlQuery query = SqliteMgr::select("select * from AssayItem order by Itemid");

    while(query.next()){
        if (language == 1){
            ItemName[query.value(1).toString()] = query.value(4).toString();
            ItemFullName[query.value(1).toString()] = query.value(5).toString();            
        }
        else {
            ItemName[query.value(1).toString()] = query.value(3).toString();
            ItemFullName[query.value(1).toString()] = query.value(6).toString();            
        }
    }

    SampleTypeArr.clear();
    SampleTypeArr<<tr("鼻咽拭子")<<tr("咽拭子");
}

void ExGlobal::updateLockTime(){
    if (LockScreenOpen == 0)
        lockscreen_time = 0;
    else if(LockScreenTime == 0)
        lockscreen_time = 60;
    else if(LockScreenTime == 1)
        lockscreen_time = 180;
    else if(LockScreenTime == 2)
        lockscreen_time = 300;
    else if(LockScreenTime == 3)
        lockscreen_time = 600;
    else if(LockScreenTime == 4)
        lockscreen_time = 900;
    else if(LockScreenTime == 5)
        lockscreen_time = 1800;
    else
        lockscreen_time = 0;
}

void ExGlobal::updatetimezone(){
    TimeZone_s=TimeZoneOpen;

    if (TimeZone == 0)  //北京时间
        system("timedatectl set-timezone 'Asia/Shanghai'");
    else if(TimeZone == 1) //东京时间
        system("timedatectl set-timezone 'Asia/Tokyo'");
    else if(TimeZone == 2)  //纽约
        system("timedatectl set-timezone 'America/New_York'");
    else if(TimeZone == 3)  //芝加哥
        system("timedatectl set-timezone 'America/Chicago'");
    else if(TimeZone == 4) //丹佛
        system("timedatectl set-timezone 'America/Denver'");
    else if(TimeZone == 5) //波哥大
        system("timedatectl set-timezone 'America/Bogota'");
    else if(TimeZone == 6)  //哈瓦那
        system("timedatectl set-timezone 'America/Havana'");
    else if(TimeZone == 7) //巴拿马
        system("timedatectl set-timezone 'America/Panama'");
    else if(TimeZone == 8) //巴黎
        system("timedatectl set-timezone 'Europe/Paris'");
    else if(TimeZone == 9) //布达佩斯
        system("timedatectl set-timezone 'Europe/Budapest'");
    else if(TimeZone == 10) //柏林
        system("timedatectl set-timezone 'Europe/Berlin'");
    else if(TimeZone == 11) //莫斯科
       system("timedatectl set-timezone 'Europe/Moscow'");
    else
       system("timedatectl set-timezone 'Asia/Shanghsi'");
    qDebug()<<"updatetimezone:::::::"<<TimeZone;
 }

QString ExGlobal::screenCapture(){
    QString filename = QCoreApplication::applicationDirPath()+"/screenshot";
    QDir dir(filename);
    if (!dir.exists())
        dir.mkpath(filename);
    filename += "/"+QDateTime::currentDateTime().toString("yyyyMMdd_hh-mm-ss-zzz")+".jpg";
    QScreen *screen = QGuiApplication::primaryScreen();
    screen->grabWindow(0).save(filename,"jpg");
    return filename;
}

void ExGlobal::setSampleCode(const QString &sampleCode){
    if (sampleCode == "p" || sampleCode == "P")
    {
        screenCapture();
    }
    /*
    else if(sampleCode.length()>1 && sampleCode[0] == 'I'){
        setSampleInfo(sampleCode.mid(1));
    }
    else if(sampleCode.length()>1 && sampleCode[0] == 'R'){
        setSampleRemark(sampleCode.mid(1));
    }
    */
    else{
        t_sampleCode = sampleCode;
        emit sampleCodeChanged();
    }

}

QStringList ExGlobal::getSampleTypeArr(){
    return SampleTypeArr;
}

QString ExGlobal::getSampleTypeName(int type){    
    return SampleTypeArr[type];
}

QString ExGlobal::getCurrSampleName(){    
    return SampleTypeArr[SampleType];
}

static QDomDocument UIXml;
void ExGlobal::UISetDefaultParam(){
    qDebug()<<"UISetDefaultParam";
    UIParam<<60;        //UI_MAIN_MENU_FONT
    UIParam<<40;        //UI_MAIN_MENU_HORIZONTALOFFSET
    UIParam<<40;        //UI_LOGIN_SUBMIT_FONT
    UIParam<<40;        //UI_IDLE_EXITLOGIN_FONT
    UIParam<<40;        //UI_IDLE_SHUTDOWN_FONT
    UIParam<<45;        //UI_BOXREADY_BUTTOM_PROMPT_FONT
    UIParam<<40;        //UI_BOXREADY_TEST_CANCEL_FONT
    UIParam<<40;        //UI_LOGIN_SUBMIT_FONT
    UIParam<<40;        //UI_LOGIN_SUBMIT_FONT
    UIParam<<40;        //UI_LOGIN_SUBMIT_FONT
    UIParam<<40;        //UI_LOGIN_SUBMIT_FONT
    UIParam<<40;        //UI_LOGIN_SUBMIT_FONT
    UIParam<<40;        //UI_LOGIN_SUBMIT_FONT
    UIParam<<40;        //UI_LOGIN_SUBMIT_FONT
    UIParam<<40;        //UI_LOGIN_SUBMIT_FONT


    QFile xmlfile(QCoreApplication::applicationDirPath()+"/UIParam.xml");
    if (!xmlfile.open(QFile::ReadOnly))
        return;
    UIXml.setContent(&xmlfile);
    xmlfile.close();
}

void ExGlobal::setUIParam(int language){
    if (UIXml.isNull())
    {
        qDebug()<<"UIXml is null";
        return;
    }
    QDomElement root = UIXml.documentElement();
    if (root.isNull())
        return;
    QDomElement param;
    if (language == 0)
        param = root.firstChildElement("cn");
    else if(language == 1)
        param = root.firstChildElement("en");
    if (param.isNull())
    {
        qDebug()<<"param is null";
        return;
    }
    QDomNode node = param.firstChild();
    while(!node.isNull()){
        QDomElement e = node.toElement();
        if (!e.isNull())
        {
            qDebug()<<e.tagName()<<e.text().toInt();
            if (e.tagName() == "UI_MAIN_MENU_FONT")
                UIParam[UI_MAIN_MENU_FONT] = e.text().toInt();
            else if(e.tagName() == "UI_MAIN_MENU_HORIZONTALOFFSET")
                UIParam[UI_MAIN_MENU_HORIZONTALOFFSET] = e.text().toInt();
            else if(e.tagName() == "UI_LOGIN_SUBMIT_FONT")
                UIParam[UI_LOGIN_SUBMIT_FONT] = e.text().toInt();
            else if(e.tagName() == "UI_IDLE_EXITLOGIN_FONT")
                UIParam[UI_IDLE_EXITLOGIN_FONT] = e.text().toInt();
            else if(e.tagName() == "UI_IDLE_SHUTDOWN_FONT")
                UIParam[UI_IDLE_SHUTDOWN_FONT] = e.text().toInt();
            else if(e.tagName() == "UI_BOXREADY_BUTTOM_PROMPT_FONT")
                UIParam[UI_BOXREADY_BUTTOM_PROMPT_FONT] = e.text().toInt();
            else if(e.tagName() == "UI_BOXREADY_TEST_CANCEL_FONT")
                UIParam[UI_BOXREADY_TEST_CANCEL_FONT] = e.text().toInt();
        }
        node = node.nextSibling();
    }
}

QString ExGlobal::buildversion(){
    qDebug()<<"projectMode="<<projectMode()<<"Ver:="<<build_version;
    if (projectMode() == 4)
        return "V1.0.16(build20201012)";
    return build_version;
}

void ExGlobal::MethodToThread(int(*start)(), const QObject *receiver, const char *method){
    QThread *thread = new QThread;
    ThreadWorker *worker = new ThreadWorker(start);
    worker->moveToThread(thread);
    connect(thread,SIGNAL(started()),worker,SLOT(start()));
    connect(worker,SIGNAL(Finish(int)),worker,SLOT(deleteLater()));
    connect(worker,SIGNAL(destroyed()),thread,SLOT(quit()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    if (receiver != nullptr && method != nullptr){
        connect(worker,SIGNAL(Finish(int)),receiver,method);
    }
    thread->start();
}

void ThreadWorker::start(){
    int result = 0;
    if (fun != nullptr)
        result = fun();
   emit Finish(result);
}
