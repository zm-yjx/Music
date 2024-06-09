#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QVector>
#include <QMediaPlayer>
#include <QString>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QImage>
#include <QPixmap>

#include "playpage.h"

class Mythread : public QThread
{
    Q_OBJECT
public:
    explicit Mythread(QObject *parent = nullptr);
public:
    QMediaPlayer *player;
    PlayPage *playpage;//设置播放器
    QString getMusicName();
    void setplaypage();//将播放器地址传给页面
public slots:
    void setname(QString x);//播放器需要对应的播放地址，在此处设置地址
    void Stop();
    QMediaPlayer* getplayer();//获取地址
private:
    QString Musicname;
    void run() override;
signals:
    void ThreadCreate();
};

#endif // MYTHREAD_H
