#ifndef FINDPAGE_H
#define FINDPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QString>
#include <QTextStream>
#include <QMediaPlayer>
#include <QFont>
#include <QDir>
#include <QComboBox>
#include <QListWidget>
#include <QMessageBox>
#include <QTcpSocket>
#include <QList>
#include <QStringList>
#include <QtCore>
#include <QDateTime>
#include <QTimer>
#include "mythread.h"
#include "playpage.h"
#include "playlist.h"

namespace Ui {
class Findpage;
}

class Findpage : public QWidget
{
    Q_OBJECT
public:
    explicit Findpage(QWidget *parent = nullptr);
    void setsocket(QTcpSocket *keysocket,QString name);
    bool selectmusic(QString path);
    ~Findpage();
public:
    QLineEdit *findname=new QLineEdit(this);
    QPushButton *Exitbtn=new QPushButton(this);
    QListWidget *Musiclist=new QListWidget(this);//设置界面控件
    PlayPage *playpage;
    QVector<QString>Musicname;
    QVector<QLineEdit*> NameEdit;
    QVector<QPushButton*> Playbtn;//界面控件存储容器
    QTimer *timer=new QTimer;
    QByteArray finally;
    QTcpSocket *findsocket;
    QString username;
    void closeEvent(QCloseEvent *event) override;
signals:
    void ThisPagefree();
    void TakedataFinish(QByteArray musicdata);

private:
    Ui::Findpage *ui;
};

#endif // FINDPAGE_H
