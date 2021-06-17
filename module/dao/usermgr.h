#ifndef USERMRG_H
#define USERMRG_H

#include <QObject>
#include"../sqlitemgr.h"
#include"../entity/User.h"

class UserMgr : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE QList<QString> getAll();

    Q_INVOKABLE bool logon(QString name, QString password);

    Q_INVOKABLE bool update(QString name, QString password);

    Q_INVOKABLE bool remove(int userid);

    Q_INVOKABLE bool insert(User* user);

signals:

public slots:
};

#endif // USERMRG_H
