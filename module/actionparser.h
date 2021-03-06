#ifndef ACTIONPARSER_H
#define ACTIONPARSER_H

#include <QByteArray>
#include <QtXml>

class ActionParser
{
public:
    ActionParser();
    static int FindVpumpParam(int PumpName);
    static QByteArray Clear_board_data(int value);
    static QByteArray ActionToByte(const QDomElement &action);
    static QByteArray ParamToByte(const QString &action, int value, int param1, int param2, int param3);
    static QByteArray ParamArrToByte(QByteArray param, int value);
    static bool pumpBottom();
private:
    static void FormatBytes(quint8 device,QByteArray &data);
    static QByteArray DrvParamToByte(const QString &action, int value, int param1, int param2, int param3);    
};

#endif // ACTIONPARSER_H
