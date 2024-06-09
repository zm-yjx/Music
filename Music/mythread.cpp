#include "mythread.h"

Mythread::Mythread(QObject *parent) : QThread(parent)
{
    this->player=new QMediaPlayer();
    this->playpage=new PlayPage();
    playpage->setplayer(player);
    emit ThreadCreate();
}

QString Mythread::getMusicName()
{
    return this->Musicname;
}

void Mythread::setplaypage()
{
    this->playpage->setplayer(this->getplayer());
}

void Mythread::setname(QString x)
{
    this->Musicname=x;
    this->playpage->Music=x;
    qDebug()<<this->playpage->Music;
}

QMediaPlayer* Mythread::getplayer()
{
    return this->player;
}

void Mythread::Stop()
{
    delete this->player;
}

void Mythread::run()
{
    QFile file1("C:/Users/33746/Desktop/MusicList.txt");
    if (!file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file for reading";
        return;
    }
    QTextStream in(&file1);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << line;
        this->playpage->PlayList->addItem(line);
    }
    file1.close();
    player->stop();
    qDebug()<<"导入";
    QString path;
    path="C:/Users/33746/Desktop/FindMusic/"+this->Musicname;
    QString Musicpath=QDir::toNativeSeparators(path);
    qDebug()<<Musicpath;
    player->setMedia(QUrl::fromLocalFile(Musicpath));
    player->setVolume(100); // 设置音量
    qDebug()<<"播放";
    player->play();
    QString imgpath;
//    imgpath="C:/Users/33746/Desktop/MusicPage/"+this->Musicname+".jpg";
    imgpath="C:/Users/33746/Desktop/MusicPage/test.jpg";
    QString Newimgpath=QDir::toNativeSeparators(imgpath);
    qDebug()<<"hhh"<<Newimgpath;
    QPixmap img(Newimgpath);
    QPixmap newimg=img.scaled(160,160);
    this->playpage->Imglabel->setPixmap(newimg);
    QRgb back_colorint = 0;
    QColor back_color;
    QPalette back_palette;
    QImage back(imgpath);
    back_colorint=back.pixel(90,90);
    qDebug()<<back_colorint;
    back_color=QColor(back_colorint);
    back_palette=this->playpage->palette();
    back_palette.setColor(QPalette::Window,QColor(back_color));
    this->playpage->setPalette(back_palette);
    connect(this->playpage->Addlistbtn,&QPushButton::clicked,[=](){
        qDebug()<<"this music add to list";
        QFile file("C:/Users/33746/Desktop/MusicList.txt");//打开存放有搜索内容文本文件
        file.open(QIODevice::Append | QIODevice::Text);//采取写模式实现往搜索内容文档中写入内容
        QString data=this->Musicname+"\n";
        QTextStream out(&file);
        out<<data;
        file.close();//写入数据
        QFile file1("C:/Users/33746/Desktop/MusicList.txt");
        if (!file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open the file for reading";
            return;
        }
        QTextStream in(&file1);
        this->playpage->PlayList->clear();
        while (!in.atEnd()) {
            QString line = in.readLine();
            qDebug() << line;
            this->playpage->PlayList->addItem(line);
        }
        file1.close();
    });
    connect(this->playpage->PlayList,&QListWidget::itemClicked,[=](QListWidgetItem *item){
        this->player->stop();
        QString name=item->text();
        qDebug()<<name;
        Mythread *Playthread=new Mythread;
        Playthread->setplaypage();
        Playthread->setname(name);
        Playthread->playpage->setWindowTitle(Playthread->getMusicName());
        Playthread->playpage->show();
        connect(Playthread->player,SIGNAL(positionChanged(qint64)),Playthread->playpage,SLOT(onPositionChanged(qint64)));
        connect(Playthread->player,SIGNAL(durationChanged(qint64)),Playthread->playpage,SLOT(onDurationChanged(qint64)));//连接播放界面的进度条控件
        int playkey=1;
        connect(Playthread->playpage->Stopbtn,&QPushButton::clicked,[=]()mutable{
            playkey=playkey+1;
            if(playkey%2==1)
            {
                Playthread->playpage->Stopbtn->setText("暂停");
                Playthread->player->play();
                qDebug()<<playkey;
            }
            else
            {
                Playthread->playpage->Stopbtn->setText("播放");
                Playthread->player->pause();
                qDebug()<<playkey;
            }
        });//连接暂停按键
        connect(Playthread->playpage,&PlayPage::Exitpage,Playthread->player,&QMediaPlayer::stop);
        Playthread->start();
    });
}
