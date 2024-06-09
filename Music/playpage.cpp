#include "playpage.h"
#include "ui_playpage.h"

PlayPage::PlayPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayPage)
{
    ui->setupUi(this);
    this->setFixedSize(785,625);
    this->Stopbtn->move(330,380);
    Stopbtn->setText("暂停");
    this->Exitbtn->move(250,380);
    Exitbtn->setText("退出");
    connect(this->Exitbtn,&QPushButton::clicked,[=](){
       emit this->Exitpage();
       this->~PlayPage();
    });
    this->Addlistbtn->move(420,380);
    Addlistbtn->setText("添加到列表");
    this->Imglabel->setFixedSize(210,210);
    this->Imglabel->move(110,150);
    this->Imglabel->setText("音乐播放");
    this->PlayList->setFixedSize(190,360);
    this->PlayList->move(520,30);//当前播放列表
}

void PlayPage::setplayer(QMediaPlayer *play)
{
    this->player=play;
}

PlayPage::~PlayPage()
{
    delete ui;
}

void PlayPage::onDurationChanged(qint64 duration)
{
    ui->horizontalSlider->setMaximum(duration); //设置进度条最大值 也就是歌曲时长 ms
    int secs = duration/1000; //全部秒数
    int mins = secs/60;//分
    secs = secs % 60;//秒
    durationTime = QString::asprintf("%d:%d",mins,secs);
    qDebug()<<positionTime;
    ui->label_2->setText(positionTime+"/"+durationTime);
}

void PlayPage::onPositionChanged(qint64 position)
{
    ui->horizontalSlider->setValue(position);
    int secs = position/1000;
    int mins = secs/60;
    secs = secs % 60;
    connect(ui->horizontalSlider,&QSlider::sliderPressed,[=](){
        connect(ui->horizontalSlider,&QSlider::sliderMoved,[=](int newvalue){
                ui->horizontalSlider->setValue(newvalue);
                this->player->setPosition(newvalue);
                this->player->play();
        });
    });
    positionTime = QString::asprintf("%d:%d",mins,secs);
    ui->label_2->setText(positionTime+"/"+durationTime);
}
