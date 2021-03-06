#ifndef USERMODEL_H
#define USERMODEL_H

#include<QAbstractListModel>
#include"../sqlitemgr.h"

class MUser{
public:
    MUser(){}
    MUser(int Userid_, QString Name_, QString Password_, QString DisplayName_, int UserType_){
        Userid = Userid_;
        Name = Name_;
        Password = Password_;
        DisplayName = DisplayName_;
        UserType = UserType_;
    }
    int Userid;
    QString Name;
    QString Password;
    QString DisplayName;
    int UserType;
};

class UserModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum LineInfo_Roles{
        RolesUserid = Qt::UserRole + 1,
        RolesName,
        RolesPassword,
        RolesDisplayName,
        RoleUserType
    };
public:
    explicit UserModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;    
    void LoadUser();
   /* Q_INVOKABLE*/   bool addUser(QString name,QString password, QString displayName, int type);
    /*Q_INVOKABLE*/   bool updateUser(QString name,QString password, QString displayName, int type);
    /*Q_INVOKABLE*/   bool deleteUser(int row);
    /*Q_INVOKABLE*/   int login(QString name,QString password);
    /*Q_INVOKABLE*/   bool updatePassword(QString oldpassword,QString newpassword);

    /*Q_INVOKABLE*/   QString getCurName(){return m_display_list[currIndex].Name;}
    /*Q_INVOKABLE*/   QString getCurDisplayName(){return m_display_list[currIndex].DisplayName;}
    /*Q_INVOKABLE*/   QString getCurPassword(){return m_display_list[currIndex].Password;}
    /*Q_INVOKABLE*/   int getCurUserType(){return m_display_list[currIndex].UserType;}
    /*Q_INVOKABLE*/   void setCurrIndex(int index){currIndex = index;}
    /*Q_INVOKABLE*/   QStringList getLogUser();
                      int getCurrIndex(){return currIndex;}
                      int getUserNum(void);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<MUser> m_display_list;
    QHash<int, QByteArray> roles;
    bool Exist(int id);
    int currIndex;
};

#endif // USERMODEL_H
