#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QListWidget>
#include <QDebug>
#include <QPushButton>
#include <QFileSystemWatcher>

#include "mythread.h"

namespace Ui {
class Playlist;
}

class Playlist : public QWidget
{
    Q_OBJECT
public:
    explicit Playlist(QWidget *parent = nullptr);
    QListWidget* PlayList=new QListWidget(this);
    QPushButton* Exitbtn=new QPushButton(this);
    ~Playlist();
private:
    Ui::Playlist *ui;
};

#endif // PLAYLIST_H
