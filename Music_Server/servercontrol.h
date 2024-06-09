#ifndef SERVERCONTROL_H
#define SERVERCONTROL_H

#include <QObject>
#include <QtCore>
#include <QTimer>

#include "mainwindow.h"
#include "findlist.h"
#include "playmusic.h"
#include "loginuser.h"
#include "adduser.h"

class ServerControl : public QObject
{
    Q_OBJECT
public:
    explicit ServerControl(QObject *parent = nullptr);
    void sendMessage(QTcpSocket *socket,QByteArray msg,QString client_tcptype);
    void readMessage();
signals:
public slots:
    void createServer();
    void closeServer();
    void newConnection();
    void disConnection();
private:
    MainWindow *widget;
    FindList *findlistdata;
    PlayMusic *playmusicdata;
    LoginUser *login;
    AddUser *addnew;
    int filelock=1;
    QString client_tcptype;
    QString client_userMsgtype;
    QString tcp_content;
    QByteArray readbuffer;
    QTcpServer *server;
    QTcpSocket *socket;
    QList <QTcpSocket*> tcplist;
};

#endif // SERVERCONTROL_H
