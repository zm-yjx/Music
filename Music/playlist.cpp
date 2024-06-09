#include "playlist.h"
#include "ui_playlist.h"

Playlist::Playlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Playlist)
{
    ui->setupUi(this);
    this->setWindowTitle("音乐列表");
    QFile file("C:/Users/33746/Desktop/MusicList.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file for reading";
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        //qDebug() << line;
        this->PlayList->addItem(line);
    }
    file.close();
    this->PlayList->setFixedSize(360,160);
    this->PlayList->move(50,50);
    this->Exitbtn->setText("<");
    this->Exitbtn->move(10,10);
    this->Exitbtn->show();//设置界面属性与位置
    connect(this->Exitbtn,&QPushButton::clicked,[=](){
        this->~Playlist();
    });//返回上一个界面
    connect(this->PlayList,&QListWidget::itemClicked,[=](QListWidgetItem *item){
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
    QFileSystemWatcher* watcher=new QFileSystemWatcher;
    watcher->addPath("C:/Users/33746/Desktop/MusicList.txt");
    connect(watcher,&QFileSystemWatcher::fileChanged,[this](){
        QFile file("C:/Users/33746/Desktop/MusicList.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open the file for reading";
            return;
        }
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            //qDebug() << line;
            this->PlayList->addItem(line);
        }
        file.close();
    });
}

Playlist::~Playlist()
{
    delete ui;
}
