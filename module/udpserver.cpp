#include "udpserver.h"
#include "log.h"
#include "uihandler.h"

#include <QDebug>
UdpServer::UdpServer(QObject *parent) : QObject(parent)
{
    m_socket = new QUdpSocket;
    m_socket->bind(8989,QUdpSocket::ShareAddress);
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(ReadData()));
    qDebug()<<"UdpServer Init"<<m_socket->localAddress().toString()<<m_socket->localPort();

    m_server = new QTcpServer;
    if (!m_server->listen(QHostAddress::Any,8686)){
        qDebug()<<m_server->errorString();
    }
    connect(m_server,SIGNAL(newConnection()),this,SLOT(NewConnect()));
    socket = new QTcpSocket;
}

void UdpServer::ReadData(){
    QByteArray array;
    QHostAddress address;
    quint16 port;
    array.resize(static_cast<int>(m_socket->bytesAvailable()));
    m_socket->readDatagram(array.data(),array.size(),&address,&port);
    qDebug()<<"ReadData"<<address<<port<<m_socket->bytesAvailable();
}

void UdpServer::NewConnect(){
    qDebug()<<"NewConnect";
    socket = m_server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()),this, SLOT(socket_Read_Data()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
}

void UdpServer::socket_Read_Data(){
    qDebug()<<"socket_Read_Data";
    QString serial = "lengxing";
    QByteArray str = socket->readAll();
    if (str == "readserial")
        socket->write(Log::readSerial().toLatin1());
    //else if(str.startsWith("uihandler"))
    //    UIHandler::sendUIMsg(str.remove(0,9));
    else if(str.length() > 4)
        Log::writeSerial(str);
    qDebug()<<str;
}

void UdpServer::disconnected(){

}
