#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include<QWidget>
#include<QThread>
#include<QTcpSocket>
#include<QHostAddress>
#include"logger.h"

class tcp_client: public QObject
{
    Q_OBJECT
public:
    explicit tcp_client(QWidget *parent = nullptr);
    void initClent();
    void updateTCP_UI();
    int connnect_state = 0;
    QString send_fromWd;
    QTcpSocket *client;
    Logger logger;
    QHostAddress address;
    unsigned short port;

signals:
    void recv_update(const QString);
    void connect_UIupdate();

};

#endif // TCP_CLIENT_H
