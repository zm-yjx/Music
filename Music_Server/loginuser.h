#ifndef LOGINUSER_H
#define LOGINUSER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QSqlError>

class LoginUser : public QObject
{
    Q_OBJECT
public:
    explicit LoginUser(QObject *parent = nullptr);
    QByteArray getLogininfo(QString user_name,QString passwwd);
public:
    QSqlDatabase db;
signals:
};

#endif // LOGINUSER_H
