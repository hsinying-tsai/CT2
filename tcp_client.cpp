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
        qDebug()<<"234"<<client->state();
    }else if(client->state()==QAbstractSocket::UnconnectedState){
        qDebug()<<"try to connect";
        client->connectToHost(address,port);
    }else{
        emit recv_update("Socket is not ConnectedState");
    }
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");

    disconnect(client, &QTcpSocket::stateChanged, this, &tcp_client::onStateChanged);
    connect(client, &QTcpSocket::stateChanged, this, &tcp_client::onStateChanged);

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
//                emit recv_update("SocketError");

            });
#else
    connect(client,&QAbstractSocket::errorOccurred,[this](QAbstractSocket::SocketError){
        emit recv_update("Socket");
    });
#endif
}

void tcp_client::updateTCP_UI()
{
    emit connect_UIupdate();
}
void tcp_client::onStateChanged(QAbstractSocket::SocketState state) {
    if(state == QAbstractSocket::ConnectedState){
        logger.writeLog(Logger::Info, "Socket connection successful.");
        connnect_state = 1;
        qDebug() << "TCP1";
    }else if(state == QAbstractSocket::UnconnectedState){
        logger.writeLog(Logger::Warning, "Socket connection failed.");
        connnect_state = 0;
        qDebug() << "TCP2";
        if(client->errorString() == "The remote host closed the connection"){
            emit recv_update("SocketError");
        }

    }
    updateTCP_UI();
}
