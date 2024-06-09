#ifndef ADDUSER_H
#define ADDUSER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QString>
#include <QSqlError>

class AddUser : public QObject
{
    Q_OBJECT
public:
    explicit AddUser(QObject *parent = nullptr);
    QByteArray getAdduserinfo(QString user_name,QString passwwd);
public:
    QSqlDatabase db;
signals:
};

#endif // ADDUSER_H
