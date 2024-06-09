#include "findpage.h"
#include "ui_findpage.h"

Findpage::Findpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Findpage)
{
    ui->setupUi(this);
    setWindowTitle("查找音乐");
    this->findname->setFixedSize(600,50);
    this->findname->move(45,69);
    this->Musiclist->setFixedSize(685,452);
    this->Musiclist->move(50,150);
    this->Exitbtn->setText("<");
    this->Exitbtn->move(10,10);
    this->Exitbtn->show();//设置界面属性与位置
    connect(this->Exitbtn,&QPushButton::clicked,[=](){
        this->hide();
    });//返回上一个界面
    connect(this->ui->pushButton,&QPushButton::clicked,[=](){
        QString data=this->findname->text();
        QString clientget;//定义搜索字符串，方便将信息发送到服务器
        if(data=="")
        {
            QMessageBox::information(this,"error","无效输入");
        }
        else
        {
            clientget="find/"+this->username+"/"+data;
            QByteArray buffer=clientget.toUtf8();
            this->findsocket->write(buffer,buffer.size());//发送检索信息到服务器
            connect(this->findsocket,&QTcpSocket::readyRead,[=](){
                QByteArray buffer=this->findsocket->readAll();
                QString str(buffer);                
                QStringList musicinfo=str.split("/");
                foreach(QString onemusicinfo,musicinfo)
                {
                    qDebug()<<"the musicinfo is:"<<onemusicinfo;
                    Musicname.append(onemusicinfo);
                    QStringList music=onemusicinfo.split(";");
                    Musiclist->addItem(music.at(0));//服务器返回两部分数据，一个是数据名字另一个是数据id但是列表只对音乐名字进行显示
                }
                disconnect(this->findsocket,&QTcpSocket::readyRead,0,0);//接受服务器返回数据后断开连接，避免信号连接到错误的槽函数
            });
            connect(this->Musiclist,&QListWidget::itemClicked,[=](QListWidgetItem *item)mutable{
                QString name=item->text();
                qDebug()<<name;
                Mythread *Playthread=new Mythread;
                Playthread->setplaypage();
                if(selectmusic("C:\\Users\\33746\\Desktop\\FindMusic\\"+name+".mp3"))//使用自定义查找函数查找本地音乐缓存是否存在
                {
                    Playthread->setname(name+".mp3");
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
                }
                else
                {
                    int index=this->Musiclist->row(item);
                    QString musickeyid,musicname;
                    qDebug()<<"not int localmusic";
                    musickeyid=(Musicname.at(index).split(";")).at(1);
                    musicname=(Musicname.at(index).split(";")).at(0);
                    qDebug()<<"the musickeyid is:"<<musickeyid;
                    clientget="play/"+this->username+"/"+musickeyid;
                    QByteArray buffer=clientget.toUtf8();
                    this->findsocket->write(buffer,buffer.size());//将查找音乐的指定id发送给服务器
                    int readnum=0;//定义接收数据包数量变量
                    timer->setInterval(1500);
                    connect(timer,&QTimer::timeout,[=](){
                        qDebug()<<"计时器触发";
                        emit this->TakedataFinish(finally);
                        timer->stop();
                    });//超时将获得数据进行处理
                    connect(this->findsocket,&QTcpSocket::readyRead,[=]()mutable{
                        if(readnum==0)
                        {
                            timer->start();
                            qDebug()<<"计时器启动";
                        }//从服务器发送数据就开始启动计时器，减少加载时间
                        readnum=readnum+1;
                        while (this->findsocket->bytesAvailable()>0)
                        {
                            buffer=this->findsocket->readAll();
                            qDebug()<<"take msg num:"<<readnum;
                            finally.append(buffer);
                        }//获取音乐数据
                    });
                    connect(this,&Findpage::TakedataFinish,[=](QByteArray writedata)mutable{
                        QFile file("C:\\Users\\33746\\Desktop\\FindMusic\\"+musicname+".mp3");
                        bool openfile=file.open(QFile::WriteOnly);
                        qDebug()<<"this music size is:"<<writedata.size();
                        qDebug()<<"the music file path is:"<<"C:\\Users\\33746\\Desktop\\FindMusic\\"+musicname+".mp3";
                        if(!openfile)
                        {
                            qDebug()<<"file open error"<<file.error()<<file.errorString();
                        }
                        file.write(writedata);
                        file.close();
                        finally.clear();
                        disconnect(this->findsocket,&QTcpSocket::readyRead,0,0);
                        disconnect(this,&Findpage::TakedataFinish,0,0);
                        disconnect(this->timer,&QTimer::timeout,0,0);
                    });//当计时器触发时，计时器处理槽函数会发送一个信号，告诉程序音乐数据已经拿到，然后通过该步骤来将对应乐曲写入到文件中，写入完成之后应该断开所有对应套接字通信与计时器的信号与槽连接，否则会使槽函数连续触发然后导致数据混乱
                }
            });
            Musicname.clear();
        }
    });
}

void Findpage::setsocket(QTcpSocket *keysocket,QString name)
{
    this->findsocket=keysocket;
    this->username=name;
}

bool Findpage::selectmusic(QString path)
{
    QDir dir("C:/Users/33746/Desktop/FindMusic");
    QStringList nameFilters=dir.entryList(QDir::Files);
    foreach(QString name,nameFilters)
    {
        qDebug()<<"select music this key is:"<<path;
        qDebug()<<"select music find key is:"<<"C:\\Users\\33746\\Desktop\\FindMusic\\"+name;
        if(path=="C:\\Users\\33746\\Desktop\\FindMusic\\"+name)
        {
            return true;
            break;
        }//通过二者文件名字查找本地缓存是否有对应歌曲
    }
    return false;
}

Findpage::~Findpage()
{
    emit ThisPagefree();
    qDebug()<<"findpage free";
}

void Findpage::closeEvent(QCloseEvent *event)
{
    this->hide();
    qDebug()<<"hide findpage free";
}
