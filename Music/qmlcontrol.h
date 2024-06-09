#ifndef QMLCONTROL_H
#define QMLCONTROL_H

#include <QObject>
#include <QDebug>
#include <QQmlContext>
#include <QColor>

class Qmlcontrol:public QObject
{
    Q_OBJECT

    Q_PROPERTY(Qmlcontrol qmlcontrol WRITE setcolor READ takemesage NOTIFE takemsg);

public:
    Qmlcontrol();
    ~Qmlcontrol();
    static Qmlcontrol*getInstance();
    void takemesage();
    void setcolor(QColor keycolor);
//    Q_INVOKABLE
signals:
    void takemsg();

private:
    QColor color;

};

#endif // QMLCONTROL_H
