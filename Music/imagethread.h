#ifndef IMAGETHREAD_H
#define IMAGETHREAD_H

#include <QObject>
#include <QThread>
#include <QDateTime>
#include <QDebug>

class Imagethread : public QThread
{
    Q_OBJECT
public:
    QDateTime *threadtimer;
    int begin;
    int now;
    Imagethread();
    void run() override;
signals:
    void timechanged();
};

#endif // IMAGETHREAD_H
