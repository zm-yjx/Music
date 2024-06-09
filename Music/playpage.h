#ifndef PLAYPAGE_H
#define PLAYPAGE_H

#include <QWidget>
#include <QDir>
#include <QDebug>
#include <QMediaPlayer>
#include <QLineEdit>
#include <QVector>
#include <QPushButton>
#include <QFont>
#include <QFileDialog>
#include <QtGlobal>
#include <QString>
#include <QSlider>
#include <QPalette>
#include <QPixmap>
#include <QLabel>
#include <QListWidget>

namespace Ui {
class PlayPage;
}

class PlayPage : public QWidget
{
    Q_OBJECT
public:
    explicit PlayPage(QWidget *parent = nullptr);
    ~PlayPage();
    void setplayer(QMediaPlayer *play);
public:
    QPushButton *Stopbtn=new QPushButton(this);
    QPushButton *Exitbtn=new QPushButton(this);
    QPushButton *Addlistbtn=new QPushButton(this);
    QLabel *Imglabel=new QLabel(this);
    QListWidget *PlayList=new QListWidget(this);//设置控件
    Ui::PlayPage *ui;
    QMediaPlayer *player;//接收外部的一个player的地址用于操作
    QString durationTime;
    QString positionTime;
    QString Music;
public slots:
    void onDurationChanged(qint64 duration); //文件时长变化，更新当前播放文件名显示
    void onPositionChanged(qint64 position); //当前文件播放位置变化，更新进度显示
signals:
    void Exitpage();
};

#endif // PLAYPAGE_H
