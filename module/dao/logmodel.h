#ifndef LOGMODEL_H
#define LOGMODEL_H


#include<QAbstractListModel>

class logdata{
public:
    logdata(){}
    logdata(int Logid_, QString LogType_, QString Result_, QString LogMsg_, QString LogIpara2_, QString LogTpara2_, QString LogTime_)
    {
        Logid = Logid_;
        LogType = LogType_;
        Result = Result_;
        LogMsg = LogMsg_;
        LogIpara2 = LogIpara2_;
        LogTpara2 = LogTpara2_;
        LogTime = LogTime_;
      }
    int Logid;
    QString LogType;
    QString Result;
    QString LogMsg;
    QString LogIpara2;
    QString LogTpara2;
    QString LogTime;
 };

class LogModel : public QAbstractListModel
{
  Q_OBJECT

public:
    enum LineInfo_Roles{
        RolesLogid = Qt::UserRole + 1,
        RolesLogType,
        RolesResult,
        RolesLogMsg,
        RolesLogIpara2,
        RolesLogTpara2,
        RoleLogTime
        };
public:
     explicit LogModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool ExistLog(int Logid);
    void InitLog();
    Q_INVOKABLE void refresh(int para);

    int getCurrLodID(){return m_display_list[currTestIndex].Logid;}
    Q_INVOKABLE QString getCurrLogType(){return m_display_list[currTestIndex].LogType;}
    Q_INVOKABLE QString getCurrLogResult(){return m_display_list[currTestIndex].Result;}
    Q_INVOKABLE QString getCurrLogMsg(){return m_display_list[currTestIndex].LogMsg;}
    Q_INVOKABLE QString getCurrLogIpara2(){return m_display_list[currTestIndex].LogIpara2;}
    Q_INVOKABLE QString getCurrLogTpara2(){return m_display_list[currTestIndex].LogTpara2;}
    Q_INVOKABLE QString getCurrValidDateTime(){return m_display_list[currTestIndex].LogTime;}

    Q_INVOKABLE void setCurrLog(int TestIndex);

protected:
    QHash<int, QByteArray> roleNames() const;


private:
    QList<logdata> m_display_list;
    QHash<int, QByteArray> roles;
    int currTestid;
    int currTestIndex;
    void resetDisplayId();

};

#endif // LOGMODEL_H
