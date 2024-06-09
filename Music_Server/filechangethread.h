#ifndef FILECHANGETHREAD_H
#define FILECHANGETHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QIODevice>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QByteArray>
#include <QSqlError>

class FileChangethread : public QThread
{
    Q_OBJECT
public:
    FileChangethread(QString username,QString begindata);
    void run() override;
private:
    QString keydatastr;
    QString username;
public:
    QByteArray find;
signals:
    void filechanged(QByteArray find);
};

#endif // FILECHANGETHREAD_H
