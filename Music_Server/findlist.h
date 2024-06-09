#ifndef FINDLIST_H
#define FINDLIST_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QString>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QTcpSocket>

#include "filechangethread.h"

class FindList :public QObject
{
    Q_OBJECT
public:
    FindList();
    void getFindinfo(QString user_name,QString findmsg);
    void setFindMsg(QString username,QString msg);
public:
    FileChangethread *filethread;
    QSqlDatabase db;
    QByteArray finddata;
signals:
    void takemsg(QByteArray finddata);
};

#endif // FINDLIST_H
