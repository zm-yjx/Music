#include "qmlcontrol.h"

Qmlcontrol::Qmlcontrol()
{

}

Qmlcontrol *Qmlcontrol::getInstance()
{
    static Qmlcontrol control;
    return &control;
}

void Qmlcontrol::takemesage()
{

}

void Qmlcontrol::setcolor(QColor keycolor)
{
    this->color=keycolor;
}
