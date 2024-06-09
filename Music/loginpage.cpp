#include "loginpage.h"
#include "ui_loginpage.h"

LoginPage::LoginPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    this->username=this->ui->lineEdit->text();
    this->userpasswd=this->ui->lineEdit_2->text();
    this->socket=this->getloginsocket();//通过单例模式来使得该程序通过唯一的一个tcp套接字来与服务器进行通信
}

void LoginPage::connectstate()
{
    QByteArray buffer=this->socket->readAll();
    QString str(buffer);
    this->loginstate=str;
    qDebug()<<"connnectstate is:"<<str;
    if(this->loginstate=="LOGIN")//接受来自服务器的登录验证字段，并且修改登录状态
    {
        QMessageBox::information(this, "infor", "登录成功！返回主界面");
        this->hide();
        disconnect(this->socket,&QTcpSocket::readyRead,0,0);
        this->tcpstate=true;
        emit finishlogin();//登录完成之后需要隐藏登录按钮
    }
    else if(this->loginstate=="ERROR")
    {
        qDebug()<<"connnectstate is error:"<<this->loginstate;
        QMessageBox::information(this, "infor", "登录失败！请检查账号与密码");
    }
}

QTcpSocket *LoginPage::getSocket()
{
    return this->socket;
}

QTcpSocket *LoginPage::getloginsocket()
{
    static QTcpSocket loginsocket;
    loginsocket.connectToHost(QHostAddress::LocalHost,11415);
    loginsocket.setReadBufferSize(8192000);
    return &loginsocket;
}//单例模式实例化唯一的tcp socket对象用于与服务器通信

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::on_pushButton_clicked()
{  
    this->username=this->ui->lineEdit->text();
    this->userpasswd=this->ui->lineEdit_2->text();
    QString buffer="login/"+username+"/"+userpasswd;
    qDebug()<<"the userinfo is"<<buffer;
    QByteArray sendbuffer=buffer.toUtf8();
    this->socket->write(sendbuffer,sendbuffer.size());
    connect(this->socket,&QTcpSocket::readyRead,[=](){
        qDebug()<<"take server message";
        this->connectstate();
    });//得到服务器响应后处理响应数据
}//登录按钮按下后处理操作

void LoginPage::on_pushButton_2_clicked()
{
    this->adduserpage=new AddUser;
    adduserpage->setAddsocket(this->socket);
    this->adduserpage->show();
    connect(this->adduserpage,&AddUser::senduserinfo,this,&LoginPage::takenewuserinfo);
}//注册按钮按下后实例化注册对应功能

void LoginPage::takenewuserinfo(QString username, QString userpasswd)
{
    this->username=username;
    this->userpasswd=userpasswd;
    this->ui->lineEdit->setText(username);
    this->ui->lineEdit_2->setText(userpasswd);
    qDebug()<<"this new username:"<<username;
    qDebug()<<"this new userpasswd:"<<userpasswd;
}//从界面获取到对应账号密码数据
