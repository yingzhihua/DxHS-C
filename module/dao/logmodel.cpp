#include "logmodel.h"
#include "../exglobal.h"
#include "../sqlitemgr.h"
#include "../sequence.h"

#include<QDebug>
LogModel::LogModel(QObject *parent):QAbstractListModel (parent)
//LogModel::LogModel(QObject *parent)
{
    roles[RolesLogid] = "Logid";
    roles[RolesLogType] = "LogType";
    roles[RolesResult] = "Result";
    roles[RolesLogMsg] = "LogMsg";
    roles[RolesLogIpara2] = "LogIpara2";
    roles[RolesLogTpara2] = "LogTpara2";
    roles[RoleLogTime] = "LogTime";
   }

QHash<int, QByteArray> LogModel::roleNames() const
{
    return roles;
}

int LogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_display_list.count();
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
    if (index.row()<0 || index.row()>=m_display_list.count())
        return QVariant();

    const logdata &test = m_display_list[index.row()];

    if (role == RolesLogid)
        return test.Logid;
    else if(role == RolesLogType)
        return test.LogType;
    else if(role == RolesResult)
        return test.Result;
    else if(role == RolesLogMsg)
        return test.LogMsg;
    else if(role == RolesLogIpara2)
        return test.LogIpara2;
    else if(role == RolesLogTpara2)
        return test.LogTpara2;
    else if(role == RoleLogTime)
        return test.LogTime;

    return QVariant();
}

void LogModel::InitLog(){
    m_display_list.clear();
    //QString sql = "select * from PanelTest where ResultType = 2 order by Testid DESC";
    QString sql = "select * from Log order by Logid DESC";
    QSqlQuery query = SqliteMgr::select(sql);
    int a;
    while(query.next()){
        logdata  logd;
        logd.Logid = query.value(0).toInt();
        a=query.value(1).toInt();
        if (a==0)
            logd.LogType ="开机";
        else if (a==1)
            logd.LogType ="关机";
        else if (a==2)
            logd.LogType ="登录";
        else if (a==3)
            logd.LogType ="错误";
        else if (a==4)
            logd.LogType ="打印";
        else if (a==5)
            logd.LogType ="审核";
        else if (a==6)
            logd.LogType ="取消审核";

        //logd.LogType = query.value(1).toString();
        a = query.value(2).toInt();
        if (a==0)
            logd.Result ="成功";
        else if (a==1)
            logd.Result ="失败";
        else
            logd.Result = query.value(2).toString();
        logd.LogMsg = query.value(4).toString();
        logd.LogIpara2 = query.value(3).toString();
        logd.LogTpara2 = query.value(5).toString();
        logd.LogTime = query.value(6).toString();
       // qDebug()<<"logd.count:"<<logd.Logid;
        m_display_list<<logd;
    }
    resetDisplayId();
    qDebug()<<"InitLogP_m_display_list.count:"<<m_display_list.count();
     qDebug()<<"InitLog_m_display_list.size:"<<m_display_list.size();
}

void LogModel::refresh(int para){
    int a;
    m_display_list.clear();
    QString sql = "select * from Log order by Logid DESC";
    QSqlQuery query = SqliteMgr::select(sql);
    beginResetModel();
    if (para==0)
      while(query.next()){
        logdata  logd;
        a=query.value(1).toInt();
        if (a==0)
            logd.LogType ="开机";
        else if (a==1)
            logd.LogType ="关机";
        else if (a==2)
            logd.LogType ="登录";
        else if (a==3)
            logd.LogType ="错误";
        else if (a==4)
            logd.LogType ="打印";
        else if (a==5)
            logd.LogType ="审核";
        else if (a==6)
            logd.LogType ="取消审核";

        //logd.LogType = query.value(1).toString();

        a = query.value(2).toInt();
        if (a==0)
            logd.Result ="成功";
        else if (a==1)
            logd.Result ="失败";
        else
            logd.Result = query.value(2).toString();
        logd.Logid = query.value(0).toInt();
        logd.LogMsg = query.value(4).toString();
        logd.LogIpara2 = query.value(3).toString();
        logd.LogTpara2 = query.value(5).toString();
        logd.LogTime = query.value(6).toString();
       //qDebug()<<"logd.count:"<<logd.Logid;
        m_display_list<<logd;
      }
    else
        while(query.next()){
          logdata  logd;
          a=query.value(1).toInt();
          if (a==(para-1))
          {
          if (a==0)
              logd.LogType ="开机";
          else if (a==1)
              logd.LogType ="关机";
          else if (a==2)
              logd.LogType ="登录";
          else if (a==3)
              logd.LogType ="错误";
          else if (a==4)
              logd.LogType ="打印";
          else if (a==5)
              logd.LogType ="审核";
          else if (a==6)
              logd.LogType ="取消审核";

          //logd.LogType = query.value(1).toString();

          a = query.value(2).toInt();
          if (a==0)
              logd.Result ="成功";
          else if (a==1)
              logd.Result ="失败";
          else
              logd.Result = query.value(2).toString();
          logd.Logid = query.value(0).toInt();
          logd.LogMsg = query.value(4).toString();
          logd.LogIpara2 = query.value(3).toString();
          logd.LogTpara2 = query.value(5).toString();
          logd.LogTime = query.value(6).toString();
         // qDebug()<<"logd.count:"<<logd.Logid;
          m_display_list<<logd;
          }
        }
    qDebug()<<"m_display_list.count:"<<m_display_list.count();
     qDebug()<<"m_display_list.size:"<<m_display_list.size();
    resetDisplayId();
    endResetModel();
}


bool LogModel::ExistLog(int Logid){
    foreach(logdata logd, m_display_list){
        if (logd.Logid == Logid)
            return true;
    }
    return false;
}

void LogModel::setCurrLog(int TestIndex){
    currTestIndex = TestIndex;
    currTestid = m_display_list[TestIndex].Logid;
   // ExGlobal::setBoxSerial(log_display_list[TestIndex].SerialNo);
   // ExGlobal::pTestResultModel->setTestid(currTestid,log_display_list[TestIndex].PanelCode);
  //  qDebug()<<"setCurrTest:"<<currTestid;
}



void LogModel::resetDisplayId(){
    int logCount = m_display_list.size();
    QList<logdata>::iterator i;
    for(i = m_display_list.begin(); i!=m_display_list.end(); i++){
        i->Logid = logCount;
        logCount--;
    }
}

void LogModel::setLogIndex(int index)
{
  currTestIndex=index;

}

int LogModel::getLogIndex(void)
{
    return  currTestIndex;

}

int LogModel::getLogNum(void)
{
    return  m_display_list.size();

}
