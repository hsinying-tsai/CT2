#include "tcp_client.h"
#include<QDebug>
#include"widget.h"

tcp_client::tcp_client(QWidget *parent)
{
    this->client = new QTcpSocket(this);
}

void tcp_client::initClent()
{
    qDebug()<<"1";
    if(client->state()==QAbstractSocket::ConnectedState){
        qDebug()<<"4";
        client->abort();
        updateTCP_UI();
    }else if(client->state()==QAbstractSocket::UnconnectedState){
        qDebug()<<"5";
        client->connectToHost(address,port);
        updateTCP_UI();
    }else{
        qDebug()<<"6";
        emit recv_update("Socket is not ConnectedState");
   }

    //recv connect
    connect(client,&QTcpSocket::readyRead,[this]{
        qDebug()<<"7";
        if(client->bytesAvailable()<=0){
            qDebug()<<"8";
            return;
        }
        const QString rev_text = QString::fromUtf8(client->readAll());
        qDebug()<< rev_text;
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
    qDebug()<<"9";
    connect(client,&QTcpSocket::connected,[this]{
        qDebug()<<"10";
        qDebug()<<"連線成功";
        logger.writeLog(Logger::Warning, "Socket connnection successful.");
        connnect_state =1;
        emit connect_UIupdate();
    });
    connect(client,&QTcpSocket::disconnected,[this]{
        qDebug()<<"11";
        qDebug()<<"連線失敗";
        logger.writeLog(Logger::Warning, "Socket connnection failed.");
        connnect_state =0;
        emit connect_UIupdate();
    });
}


