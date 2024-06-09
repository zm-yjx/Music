#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setmouse();
    ui->setupUi(this);
    this->timerofimage=new Imagethread();
    this->timerofimage->setParent(this);
    QFile file("C:/Users/33746/Desktop/MusicList.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file for reading";
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        this->Musicname.push_back(line);
        this->musiclistMax=this->musiclistMax+1;
    }
    file.close();
    setAttribute(Qt::WA_Hover,true);//设置鼠标事件
    ui->pushButton_15->hide();
    ui->pushButton_16->hide();
    ui->pushButton_17->hide();
    ui->pushButton_18->hide();
    ui->MainwidgetSlider->hide();
    ui->Musictimelabel_14->hide();
    ui->label_14->hide();
    ui->label_2->setPixmap(QPixmap(":/logo.png"));
    ui->label_2->resize(50,50);
    ui->pushButton->setText(tr("发现音乐"));
    ui->pushButton->setStyleSheet("background-color: gray;color: white;");
    ui->pushButton_2->setText(tr("本地音乐"));
    ui->pushButton_2->setStyleSheet("background-color: gray;color: white;");
    ui->pushButton_3->setText(tr("关注"));
    ui->pushButton_3->setStyleSheet("background-color: gray;color: white;");
    ui->pushButton_4->setText(tr("商城"));
    ui->pushButton_4->setStyleSheet("background-color: gray;color: white;");
    ui->pushButton_5->setText(tr("音乐人"));
    ui->pushButton_5->setStyleSheet("background-color: gray;color: white;");
    ui->pushButton_6->setText(tr("云推歌"));
    ui->pushButton_6->setStyleSheet("background-color: gray;color: white;");
    ui->pushButton_9->setStyleSheet("background-color: gray;color: white;");
    ui->pushButton_10->setStyleSheet("background-color: gray;color: white;");
    ui->pushButton_11->setStyleSheet("background-color: gray;color: white;");
    ui->pushButton_12->setStyleSheet("background-color: gray;color: white;");
    ui->pushButton_13->setStyleSheet("background-color: gray;color: white;");
    ui->pushButton_15->setText("循环列表");
    ui->pushButton_16->setText("播放");
    ui->pushButton_17->setText("下一首");
    font.setPointSize(18);
    ui->label->setStyleSheet("font-size: 20px;");
    ui->label_4->setPixmap(QPixmap(":/test3.jpg"));
    //QGraphicsBlurEffect *effetc=new QGraphicsBlurEffect(ui->label_4);
    //effetc->setBlurRadius(2);
    //ui->label_4->setGraphicsEffect(effetc);//设置虚化
    ui->label_5->setStyleSheet("font-size: 32px;");
    ui->pushButton_7->setText(tr("<"));
    ui->pushButton_7->setFont(font);
    ui->pushButton_7->setStyleSheet("background-color: white;");
    ui->pushButton_8->setText(tr(">"));
    ui->pushButton_8->setFont(font);
    font.setPointSize(7);
    ui->pushButton_15->setFont(font);
    ui->pushButton_16->setFont(font);
    ui->pushButton_17->setFont(font);
    ui->pushButton_8->setStyleSheet("background-color: white;");
    ui->pushButton_7->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    ui->pushButton_8->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    QString imgdata=":/test";
    imgnum=2;
    QString imgdata2=".jpg";
    QImage back(":/test3.jpg");
    this->back_colorint=back.pixel(10,10);
    this->back_color=QColor(this->back_colorint);
    back_palette=this->palette();
    back_palette.setColor(QPalette::Window,QColor(this->back_color));
    this->setPalette(back_palette);
    QColor change=this->changecolor(back_color);
    this->ui->pushButton_7->setStyleSheet("background-color: "+change.name()+";");
    this->ui->pushButton_8->setStyleSheet("background-color: "+change.name()+";");
    QPalette pal ;
    pal=this->ui->label->palette();
    pal.setColor(QPalette::WindowText, change);
    this->ui->label->setPalette(pal);
    pal=this->ui->label_3->palette();
    pal.setColor(QPalette::WindowText, change);
    this->ui->label_3->setPalette(pal);
    pal = this->ui->label_5->palette();
    pal.setColor(QPalette::WindowText, change);
    this->ui->label_5->setPalette(pal);
    this->quickView=new QQuickView();
    this->quickView->setSource(QUrl("qrc:/Mainpage.qml"));
    this->WindowContainer=QWidget::createWindowContainer(quickView, this);
    this->quickView->setResizeMode(QQuickView::SizeRootObjectToView);
    this->pRoot=(QObject*)this->quickView->rootObject();//接入qml文件书写的控件
    WindowContainer->move(36, 430); // 设置x,y坐标
    WindowContainer->resize(965, 205); // 设置qml控件宽度和高度
    if (pRoot != NULL) {
        connect(pRoot, SIGNAL(sendMsg(QString)), this, SLOT(qmltakemsg(QString)));
        connect(this,SIGNAL(sendcolor(QString)),pRoot,SIGNAL(takeColor(QString)));
        connect(this,SIGNAL(sendtextcolor(QString)),pRoot,SIGNAL(taketextColor(QString)));
    }
    emit this->sendcolor(this->back_color.name());
    emit this->sendtextcolor(change.name());//设置qml与对应c++层需要关联的信号与槽
    timerofimage->start();
    connect(ui->pushButton_7,&QPushButton::clicked,ui->label_4,[=](){
        if(imgnum>0)
        {
          imgnum=imgnum-1;
        }
        QString num;
        num=QString::number(imgnum);
        imgpath=imgdata+num+imgdata2;
        qDebug()<<imgpath;
        ui->label_4->setPixmap(QPixmap(imgpath));
        QImage back(imgpath);
        this->back_colorint=back.pixel(10,10);
        qDebug()<<this->back_colorint;
        this->back_color=QColor(this->back_colorint);
        back_palette=this->palette();
        back_palette.setColor(QPalette::Window,QColor(this->back_color));
        this->setPalette(back_palette);
        qDebug()<<"now color is"<<back_color.name();
        QColor change=this->changecolor(back_color);
        QPalette pal ;
        pal=this->ui->label->palette();
        pal.setColor(QPalette::WindowText, change);
        this->ui->label->setPalette(pal);
        pal=this->ui->label_5->palette();
        pal.setColor(QPalette::WindowText, change);
        this->ui->label_5->setPalette(pal);
        pal=this->ui->label_3->palette();
        pal.setColor(QPalette::WindowText, change);
        this->ui->label_3->setPalette(pal);
        this->ui->pushButton_7->setStyleSheet("background-color: "+change.name()+";");
        this->ui->pushButton_8->setStyleSheet("background-color: "+change.name()+";");
        emit this->sendcolor(this->back_color.name());
        emit this->sendtextcolor(change.name());
    });
    connect(ui->pushButton_8,&QPushButton::clicked,ui->label_4,[=](){
        if(imgnum<7)
        {
            imgnum=imgnum+1;
        }
        QString num;
        num=QString::number(imgnum);
        imgpath=imgdata+num+imgdata2;
        ui->label_4->setPixmap(QPixmap(imgpath));
        QImage back(imgpath);
        this->back_colorint=back.pixel(10,10);
        this->back_color=QColor(this->back_colorint);
        back_palette=this->palette();
        back_palette.setColor(QPalette::Window,QColor(this->back_color));
        this->setPalette(back_palette);
        QColor change=this->changecolor(back_color);
        QPalette pal ;
        pal = this->ui->label->palette();
        pal.setColor(QPalette::WindowText, change);
        this->ui->label->setPalette(pal);
        pal = this->ui->label_5->palette();
        pal.setColor(QPalette::WindowText, change);
        this->ui->label_5->setPalette(pal);
        pal = this->ui->label_3->palette();
        pal.setColor(QPalette::WindowText, change);
        this->ui->label_3->setPalette(pal);
        this->ui->pushButton_7->setStyleSheet("background-color: "+change.name()+";");
        this->ui->pushButton_8->setStyleSheet("background-color: "+change.name()+";");
        emit this->sendcolor(this->back_color.name());
        emit this->sendtextcolor(change.name());
    });//图片切换
    connect(this->timerofimage,&Imagethread::timechanged,[=]()mutable{
        imgnum=(imgnum)%7+1;
        QString num;
        num=QString::number(imgnum);
        imgpath=imgdata+num+imgdata2;
        ui->label_4->setPixmap(QPixmap(imgpath));
        QImage back(imgpath);
        this->back_colorint=back.pixel(10,10);
        this->back_color=QColor(this->back_colorint);
        back_palette=this->palette();
        back_palette.setColor(QPalette::Window,QColor(this->back_color));
        this->setPalette(back_palette);
        QColor change=this->changecolor(back_color);
        this->ui->pushButton_7->setStyleSheet("background-color: "+change.name()+";");
        this->ui->pushButton_8->setStyleSheet("background-color: "+change.name()+";");
        QPalette pal = this->ui->label->palette();
        pal.setColor(QPalette::WindowText, change);
        this->ui->label->setPalette(pal);
        pal = this->ui->label_5->palette();
        pal.setColor(QPalette::WindowText, change);
        this->ui->label_5->setPalette(pal);
        pal=this->ui->label_3->palette();
        pal.setColor(QPalette::WindowText, change);
        this->ui->label_3->setPalette(pal);
        emit this->sendcolor(this->back_color.name());
        emit this->sendtextcolor(change.name());
    });//多线程实现图片自动但是不阻塞主线程切换
    connect(ui->lineEdit,&QLineEdit::returnPressed,[=](){
        qDebug()<<this->loginState;
        if(this->loginState)
        {
            if(this->Findwidget==nullptr)
            {
                this->Findwidget=new Findpage;
            }
            this->Findwidget->setsocket(this->mainsocket,this->username);
            qDebug()<<"当前缓存区大小:"<<this->Findwidget->findsocket->readBufferSize();
            this->Findwidget->setPalette(this->back_palette);
            this->Findwidget->setFixedSize(785,625);
            this->Findwidget->show();
            this->find_music(ui->lineEdit->text());
        }
        else
        {
            QMessageBox::information(this, " error", "请检测网络连接");
        }
    });//搜索框转入到查找音乐界面
    connect(ui->pushButton,&QPushButton::clicked,[=](){
        qDebug()<<this->loginState;
        if(this->loginState)
        {
            if(this->Findwidget==nullptr)
            {
                this->Findwidget=new Findpage;
            }
            this->Findwidget->setsocket(this->mainsocket,this->username);
            qDebug()<<"当前缓存区大小:"<<this->Findwidget->findsocket->readBufferSize();
            this->Findwidget->setPalette(this->back_palette);
            this->Findwidget->setFixedSize(785,625);
            this->Findwidget->show();
            this->find_music(ui->lineEdit->text());
        }
        else
        {
            QMessageBox::information(this, " error", "请检测网络连接");
        }
    });//发现音乐转入到查找音乐界面
    connect(ui->pushButton_2,&QPushButton::clicked,[=](){
        this->Mymusicpage=new MyMusic;
        this->Mymusicpage->setPalette(this->back_palette);
        this->Mymusicpage->setFixedSize(785,625);
        this->Mymusicpage->show();
    });//转到我的音乐界面
    connect(ui->pushButton_3,&QPushButton::clicked,[=](){
        QUrl url("https://music.163.com/#/friend");
        QDesktopServices::openUrl(url);
    });//转到我的关注界面
    connect(ui->pushButton_4,&QPushButton::clicked,[=](){
        QUrl url("https://music.163.com/store/product");
        QDesktopServices::openUrl(url);
    });//转到我的商场界面
    connect(ui->pushButton_5,&QPushButton::clicked,[=](){
        QUrl url("https://music.163.com/st/musician");
        QDesktopServices::openUrl(url);
    });//转到音乐人界面
    connect(ui->pushButton_6,&QPushButton::clicked,[=](){
        QUrl url("https://music.163.com/st/ad-song");
        QDesktopServices::openUrl(url);
    });//转到云推歌界面
    connect(ui->pushButton_9,&QPushButton::clicked,[=](){
        QUrl url("https://music.163.com/#/discover/playlist/?cat=%E5%8D%8E%E8%AF%AD");
        QDesktopServices::openUrl(url);
    });//转到华语歌界面
    connect(ui->pushButton_10,&QPushButton::clicked,[=](){
        QUrl url("https://music.163.com/#/discover/playlist/?cat=%E6%B5%81%E8%A1%8C");
        QDesktopServices::openUrl(url);
    });//转到流行歌界面
    connect(ui->pushButton_11,&QPushButton::clicked,[=](){
        QUrl url("https://music.163.com/#/discover/playlist/?cat=%E6%91%87%E6%BB%9A");
        QDesktopServices::openUrl(url);
    });//转到摇滚歌界面
    connect(ui->pushButton_12,&QPushButton::clicked,[=](){
        QUrl url("https://music.163.com/#/discover/playlist/?cat=%E6%B0%91%E8%B0%A3");
        QDesktopServices::openUrl(url);
    });//转到民谣歌界面
    connect(ui->pushButton_13,&QPushButton::clicked,[=](){
        QUrl url("https://music.163.com/#/discover/playlist/?cat=%E7%94%B5%E5%AD%90");
        QDesktopServices::openUrl(url);
    });//转到电子歌界面
    connect(ui->pushButton_14,&QPushButton::clicked,[=](){
        this->Playmusiclist=new Playlist();
        Playmusiclist->show();
    });//显示播放列表
    int playsetstats=0;
    connect(ui->pushButton_15,&QPushButton::clicked,[=]()mutable{
        playsetstats=playsetstats+1;
        QFile file("C:/Users/33746/Desktop/MusicList.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open the file for reading";
            return;
        }
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            this->Musicname.push_back(line);
            this->musiclistMax=this->musiclistMax+1;
        }
        file.close();
        if(this->player==nullptr)
        {
           this->player=new QMediaPlayer;
        }
        if(playsetstats%2==1)
        {
            this->ui->pushButton_15->setText("循环列表");
            this->Playstate=1;
            QString path;
            path="C:/Users/33746/Desktop/FindMusic/"+Musicname.at(musicnum);
            this->ui->label_14->setText(Musicname.at(musicnum));
            QString Musicpath=QDir::toNativeSeparators(path);
            qDebug()<<Musicpath;
            player->setMedia(QUrl::fromLocalFile(Musicpath));
            player->setVolume(100); // 设置音量
            connect(this->player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
            connect(this->player,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));//连接播放界面的进度条控件
            player->play();
        }
        else if(playsetstats%2==0)
        {
            ui->pushButton_15->setText("单一循环");
            this->Playstate=0;
            QString path;
            path="C:/Users/33746/Desktop/FindMusic/"+Musicname.at(musicnum);
            this->ui->label_14->setText(Musicname.at(musicnum));
            QString Musicpath=QDir::toNativeSeparators(path);
            qDebug()<<Musicpath;
            if(this->player==nullptr)
            {
               this->player=new QMediaPlayer;
            }
            player->setMedia(QUrl::fromLocalFile(Musicpath));
            player->setVolume(100); // 设置音量
            connect(this->player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
            connect(this->player,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));//连接播放界面的进度条控件
            player->play();
        }
    });//设置循环方式
    int playstats=0;
    connect(ui->pushButton_16,&QPushButton::clicked,[=]()mutable{
        playstats=playstats+1;
        if(this->player==nullptr)
        {
            this->player=new QMediaPlayer();
            qDebug()<<"create new player";
            QFile file("C:/Users/33746/Desktop/MusicList.txt");
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "Failed to open the file for reading";
                return;
            }
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                this->Musicname.push_back(line);
            }
            file.close();
            QString path;
            path="C:/Users/33746/Desktop/FindMusic/"+Musicname.at(this->musicnum);
            this->ui->label_14->setText(Musicname.at(this->musicnum));
            QString Musicpath=QDir::toNativeSeparators(path);
            qDebug()<<Musicpath;
            player->setMedia(QUrl::fromLocalFile(Musicpath));
            player->setVolume(100); // 设置音量
            qDebug()<<"播放";
        }
        if(playstats%2==1)
        {
            ui->pushButton_16->setText("暂停");
            player->play();
            connect(this->player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
            connect(this->player,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));//连接播放界面的进度条控件
        }
        else if (playstats%2==0)
        {
            ui->pushButton_16->setText("播放");
            player->pause();
            connect(this->player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
            connect(this->player,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));//连接播放界面的进度条控件
        }
    });//从列表第一个开始播放
    connect(ui->pushButton_17,&QPushButton::clicked,[=](){
            qDebug()<<"add music list";
            QFile file("C:/Users/33746/Desktop/MusicList.txt");
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "Failed to open the file for reading";
                return;
            }
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                this->Musicname.push_back(line);
            }
            file.close();
        if(this->musicnum<this->musiclistMax-1)
        {
            this->musicnum=this->musicnum+1;
            if(this->player!=nullptr)
            {
                this->player->stop();
            }
            else {
                this->player=new QMediaPlayer(this);
            }
            QString path;
            path="C:/Users/33746/Desktop/FindMusic/"+Musicname.at(musicnum);
            this->ui->label_14->setText(Musicname.at(musicnum));
            QString Musicpath=QDir::toNativeSeparators(path);
            qDebug()<<Musicpath;
            player->setMedia(QUrl::fromLocalFile(Musicpath));
            player->setVolume(100); // 设置音量
            qDebug()<<"播放";
            player->play();
            connect(this->player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
            connect(this->player,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));//连接播放界面的进度条控件
        }
        else
        {
            this->musicnum=0;
            if(this->player!=nullptr)
            {
                this->player->stop();
            }
            else {
                this->player=new QMediaPlayer(this);
            }
            QString path;
            path="C:/Users/33746/Desktop/FindMusic/"+Musicname.at(musicnum);
            this->ui->label_14->setText(Musicname.at(musicnum));
            QString Musicpath=QDir::toNativeSeparators(path);
            qDebug()<<Musicpath;
            player->setMedia(QUrl::fromLocalFile(Musicpath));
            player->setVolume(100); // 设置音量
            qDebug()<<"播放";
            player->play();
            connect(this->player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
            connect(this->player,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));//连接播放界面的进度条控件
        }
    });//下一首播放
    connect(this->ui->pushButton_18,&QPushButton::clicked,[=](){
        QFile file("C:/Users/33746/Desktop/MusicList.txt");//打开存放有搜索内容文本文件
        file.open(QIODevice::Append | QIODevice::Text);//采取写模式实现往搜索内容文档中写入内容
        QString data=this->Musicname.at(musicnum) +"\n";
        QTextStream out(&file);
        out<<data;
        file.close();//写入数据
        QFile file1("C:/Users/33746/Desktop/MusicList.txt");
        if (!file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open the file for reading";
            return;
        }
        QTextStream in(&file1);
        file1.close();
    });//添加到播放列表
    connect(this->ui->pushButton_19,&QPushButton::clicked,[=](){
        this->LoginServer=new LoginPage;
        this->LoginServer->show();
        connect(this->LoginServer,&LoginPage::finishlogin,[=](){
            this->ui->pushButton_19->hide();
            this->mainsocket=this->LoginServer->getSocket();         
            this->username=this->LoginServer->username;
            this->loginState=this->LoginServer->tcpstate;
            qDebug()<<"this page login state:"<<this->loginState;
        });//当服务器确定登录操作完成后处理对应数据关联
    });//未登录时的登录注册按钮
}

QColor MainWindow::changecolor(QColor color)
{
    return QColor(255-color.red(),255-color.green(),255-color.blue());
}

QSlider *MainWindow::getSlider()
{
    return this->ui->MainwidgetSlider;
}

QLabel *MainWindow::getlabel()
{
    return  this->ui->Musictimelabel_14;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::find_music(QString x)
{
    this->Findwidget->findname->setText(x);
    qDebug()<<this->Findwidget->findname->text();
}

void MainWindow::onDurationChanged(qint64 duration)
{
    QSlider *Play;
    QLabel *Playlabel;
    Play=this->getSlider();
    Play->setMaximum(duration); //设置进度条最大值 也就是歌曲时长 ms
    int secs = duration/1000; //全部秒数
    this->Maxtime=secs;
    int mins = secs/60;//分
    secs = secs % 60;//秒
    durationTime = QString::asprintf("%d:%d",mins,secs);
    Playlabel=this->getlabel();
    Playlabel->setText(positionTime+"/"+durationTime);
}

void MainWindow::onPositionChanged(qint64 position)
{
    QSlider *Play;
    QLabel *Playlabel;
    Play=this->getSlider();
    Play->setValue(position);
    int secs = position/1000;
    this->Nowtime=secs;
    int mins = secs/60;
    secs = secs % 60;
    connect(Play,&QSlider::sliderPressed,[=](){
        connect(Play,&QSlider::sliderMoved,[=](int newvalue){
                Play->setValue(newvalue);
                this->player->setPosition(newvalue);
                this->player->play();
        });
    });
    positionTime = QString::asprintf("%d:%d",mins,secs);
    Playlabel=this->getlabel();
    Playlabel->setText(positionTime+"/"+durationTime);
    if(this->Nowtime>=this->Maxtime)
    {
        if(this->Playstate==0)
        {
            QString path;
            path="C:/Users/33746/Desktop/FindMusic/"+Musicname.at(musicnum);
            QString Musicpath=QDir::toNativeSeparators(path);
            qDebug()<<Musicpath;
            player->setMedia(QUrl::fromLocalFile(Musicpath));
            player->setVolume(100); // 设置音量
            connect(this->player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
            connect(this->player,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));//连接播放界面的进度条控件
            player->play();
        }
        else if(this->Playstate==1&&this->musicnum<this->musiclistMax-1)
        {
            musicnum=musicnum+1;
            QString path;
            path="C:/Users/33746/Desktop/FindMusic/"+Musicname.at(musicnum);
            QString Musicpath=QDir::toNativeSeparators(path);
            qDebug()<<Musicpath;
            player->setMedia(QUrl::fromLocalFile(Musicpath));
            player->setVolume(100); // 设置音量
            connect(this->player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
            connect(this->player,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));//连接播放界面的进度条控件
            player->play();
        }
    }
}

void MainWindow::qmltakemsg(QString text)
{
    qDebug()<<"qml text is:"<<text;
}

void MainWindow::setmouse()
{
    this->setMouseTracking(true);
}

bool MainWindow::event(QEvent *event)
{
    int ykey=0;
    if(QEvent::HoverMove == event->type())//鼠标移动
    {
        QHoverEvent *hoverEvent = static_cast<QHoverEvent*>(event);
        ykey=hoverEvent->pos().y();
        if(ykey>580)
        {
            this->ui->pushButton_15->show();
            this->ui->pushButton_16->show();
            this->ui->pushButton_17->show();
            this->ui->pushButton_18->show();
            this->ui->MainwidgetSlider->show();
            this->ui->Musictimelabel_14->show();
            this->ui->label_14->show();
        }
        else
        {
            ui->pushButton_15->hide();
            ui->pushButton_16->hide();
            ui->pushButton_17->hide();
            ui->pushButton_18->hide();
            ui->MainwidgetSlider->hide();
            ui->Musictimelabel_14->hide();
            ui->label_14->hide();
        }
    }
    return QWidget::event(event);
}
