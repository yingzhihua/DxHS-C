#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>

class UdpServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpServer(QObject *parent = nullptr);

signals:

public slots:
    void ReadData();
    void NewConnect();
    void socket_Read_Data();
    void disconnected();

private:
    QUdpSocket *m_socket;
    QTcpServer *m_server;
    QTcpSocket *socket;

};

#endif // UDPSERVER_H
