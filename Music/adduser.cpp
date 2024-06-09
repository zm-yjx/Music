#include "adduser.h"
#include "ui_adduser.h"

AddUser::AddUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);
}

QString AddUser::getnewusername()
{
    return  this->username;
}

QString AddUser::getpasswd()
{
    return this->userpasswd;
}

void AddUser::setAddsocket(QTcpSocket *keysocket)
{
    this->addsocket=keysocket;
}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::on_pushButton_clicked()
{
    this->username=this->ui->lineEdit->text();
    this->userpasswd=this->ui->lineEdit_2->text();
    if(this->username!=""&&this->userpasswd!="")
    {
        QString buffer="adduser/"+username+"/"+userpasswd;
        qDebug()<<"the userinfo is"<<buffer;
        QByteArray sendbuffer=buffer.toUtf8();
        this->addsocket->write(sendbuffer,sendbuffer.size());
        connect(this->addsocket,&QTcpSocket::readyRead,[=](){
            QByteArray buffer1=this->addsocket->readAll();
            QString str(buffer1);
            if(this->ui->lineEdit_2->text()==this->ui->lineEdit_3->text()&&str=="ADDUSER")
            {
                QMessageBox::information(this, "infor", "注册成功！返回登录界面");
                emit senduserinfo(this->username,this->userpasswd);
                this->hide();
                disconnect(this->addsocket,&QTcpSocket::readyRead,0,0);
            }
            else
            {
                QMessageBox::information(this, "infor", "注册失败，请检测账号密码");
                this->ui->lineEdit_3->clear();
            }
        });
    }
    else
    {
        QMessageBox::information(this, "infor", "注册失败，请检测账号密码");
    }
}
