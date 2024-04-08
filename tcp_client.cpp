#include "tcp_client.h"
#include<QDebug>
#include"widget.h"

tcp_client::tcp_client(QWidget *parent)
{
    this->client = new QTcpSocket(this);
}

void tcp_client::initClent()
{
    if(client->state()==QAbstractSocket::ConnectedState){
        client->abort();
        updateTCP_UI();
    }else if(client->state()==QAbstractSocket::UnconnectedState){
        client->connectToHost(address,port);
        updateTCP_UI();
    }else{
        emit recv_update("Socket is not ConnectedState");
   }

    //recv connect
    connect(client,&QTcpSocket::readyRead,[this]{
        if(client->bytesAvailable()<=0){
            return;
        }
        const QString rev_text = QString::fromUtf8(client->readAll());
//        qDebug()<< rev_text;
        emit recv_update(rev_text.toUtf8());
    });
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
    connect(client, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            [this](QAbstractSocket::SocketError){
                emit recv_update("SocketError");
            });
#else
    connect(client,&QAbstractSocket::errorOccurred,[this](QAbstractSocket::SocketError){
        emit recv_update("SocketError");
    });
#endif
}

void tcp_client::updateTCP_UI()
{
    connect(client,&QTcpSocket::connected,[this]{
        logger.writeLog(Logger::Warning, "Socket connnection successful.");
        connnect_state =1;
        emit connect_UIupdate();
    });
    connect(client,&QTcpSocket::disconnected,[this]{
        logger.writeLog(Logger::Warning, "Socket connnection failed.");
        connnect_state =0;
        emit connect_UIupdate();
    });
}


