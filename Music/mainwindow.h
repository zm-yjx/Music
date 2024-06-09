#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QDateTime>
#include <QString>
#include <QAction>
#include <QDebug>
#include <QFont>
#include <QVector>
#include <QSlider>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QGraphicsEffect>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QPalette>
#include <QQuickView>
#include <QTcpSocket>

#include "findpage.h"
#include "mymusic.h"
#include "playlist.h"
#include "imagethread.h"
#include "qmlcontrol.h"
#include "loginpage.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    QColor changecolor(QColor color);
    QSlider *getSlider();
    QLabel *getlabel();
    ~MainWindow();
private: 
    QQuickView *quickView;
    QWidget *WindowContainer;
    QObject *pRoot;
    int imgnum;
    int Maxtime;
    int Nowtime;
    int musicnum=0;
    int musiclistMax=0;
    int Playstate=0;
    QRgb back_colorint = 0;
    QColor back_color;
    QVector<QString> Musicname;
    QString imgpath;
    QPalette back_palette;
    QFont font;
    Ui::MainWindow *ui;
    QMediaPlayer *player=nullptr;
    QString durationTime;
    QString positionTime;
    Imagethread *timerofimage;
    bool loginState=false;
    Findpage *Findwidget=nullptr;
    MyMusic *Mymusicpage=nullptr;
    Playlist *Playmusiclist;
    LoginPage *LoginServer;
    QTcpSocket *mainsocket;
    QString username;
    QString userpasswd;
public slots:
    void find_music(QString x);
    void onDurationChanged(qint64 duration); //文件时长变化，更新当前播放文件名显示
    void onPositionChanged(qint64 position); //当前文件播放位置变化，更新进度显示
    void qmltakemsg(QString text);
signals:
    void sendcolor(QString color);
    void sendtextcolor(QString color);
private:
    void setmouse();
    bool event(QEvent *event) override;
};
#endif // MAINWINDOW_H
