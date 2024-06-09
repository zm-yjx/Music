#ifndef PLAYMUSIC_H
#define PLAYMUSIC_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QString>
#include <QUrl>
#include <QFile>

class PlayMusic :QObject
{
    Q_OBJECT
public:
    PlayMusic();    
    QByteArray getPlayinfo(QString musicid);
public:
    QSqlDatabase db;
};

#endif // PLAYMUSIC_H
