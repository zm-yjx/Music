#include "imagethread.h"

Imagethread::Imagethread()
{

}

void Imagethread::run()
{
    qDebug()<<"thread begin";
    this->threadtimer=new QDateTime(QDateTime::currentDateTime());
    this->begin=this->threadtimer->toTime_t();
    while (1)
    {
        this->sleep(3);
        emit timechanged();
    }
}
