#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QListWidget>

class Mylistwidget : public QListWidgetItem
{
    Q_OBJECT
public:
    Mylistwidget();
    QSize sizeHint()
    {
       return QSize(100, 50);
    }
    ~Mylistwidget();
};

#endif // MYLISTWIDGET_H
