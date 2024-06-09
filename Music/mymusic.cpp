#include "mymusic.h"
#include "ui_mymusic.h"

MyMusic::MyMusic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyMusic)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("我的音乐"));
    this->Exitbtn->setText("<");
    this->Exitbtn->move(10,10);
    this->Exitbtn->show();
    this->Musiclist->setFixedSize(800,452);
    this->Musiclist->move(45,50);
    connect(this->Exitbtn,&QPushButton::clicked,[=](){
       this->~MyMusic();
    });//返回上一个界面
    QDir dir("C:/Users/33746/Desktop/FindMusic");
    QDir::Filters filters = QDir::Files | QDir::NoDotAndDotDot;
    QStringList nameFilters;
    nameFilters << "*.mp3";//创建过滤器，只包含指定后缀的文件
    QFileInfoList fileList = dir.entryInfoList(nameFilters, filters);
    this->Musiclist->setParent(this);
    QString styleSheet = "QListWidget {"
                          "background-color: white;"
                          "color: black;"
                          "} ";
    this->Musiclist->setStyleSheet(styleSheet);
    this->Musiclist->setFixedSize(685,525);
    foreach (QFileInfo fileInfo, fileList) {
        Musicname.append(fileInfo.fileName());
        Musiclist->addItem(fileInfo.fileName());
        qDebug() << fileInfo.fileName();
        qDebug()<<fileInfo.path();
    }//遍历获取的mp3文件
    connect(this->Musiclist,&QListWidget::itemClicked,[=](QListWidgetItem *item){
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
    Musicname.clear();
}

MyMusic::~MyMusic()
{
    qDebug()<<this;
    qDebug()<<"delete mymusic";
}

void MyMusic::closeEvent(QCloseEvent *event)
{
    this->hide();
}
