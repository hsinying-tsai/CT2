#include "tcp_client.h"
#include<QDebug>
#include"widget.h"

tcp_client::tcp_client(QWidget *parent)
{
    this->client = new QTcpSocket(this);
    // initClent();
}

void tcp_client::initClent()
{
    qDebug()<<"1";
    if(client->state()==QAbstractSocket::ConnectedState){
        qDebug()<<"2";
        qDebug()<<"initClient()_unconnect";
    }else if(client->state()==QAbstractSocket::UnconnectedState){
        qDebug()<<"3";
        qDebug()<<"initClient()_connect";
    }

    if(client->state()==QAbstractSocket::ConnectedState){
        qDebug()<<"4";
        client->abort();
        emit recv_update("Connection terminated");
        updateTCP_UI();
    }else if(client->state()==QAbstractSocket::UnconnectedState){
        qDebug()<<"5";
        client->connectToHost(address,port);
        emit recv_update("Connection successful");
        updateTCP_UI();
    }else{
        qDebug()<<"6";
        emit recv_update("It is not ConnectedState");
        // ui->textRecv->append("It is not ConnectedState");
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
                emit recv_update("Socket Error:"+client->errorString());
            });
#else
    connect(client,&QAbstractSocket::errorOccurred,[this](QAbstractSocket::SocketError){
        qDebug()<<"18";
        emit recv_update("Socket Error:"+client->errorString());
    });
#endif
}

void tcp_client::updateTCP_UI()
{
    qDebug()<<"9";
    connect(client,&QTcpSocket::connected,[this]{
        //連線成功
        qDebug()<<"10";
        qDebug()<<"連線成功";
        connnect_state =1;
        emit connect_UIupdate();
    });
    connect(client,&QTcpSocket::disconnected,[this]{
        //連線失敗
        qDebug()<<"11";
        qDebug()<<"連線失敗";
        connnect_state =0;
        emit connect_UIupdate();
    });
}


