#include "servercontrol.h"

ServerControl::ServerControl(QObject *parent) : QObject(parent)
{
    this->widget=new MainWindow;
    connect(this->widget->getcreatbtn(),&QPushButton::clicked,this,&ServerControl::createServer);
    connect(this->widget->getclosebtn(),&QPushButton::clicked,this,&ServerControl::closeServer);
    this->widget->show();
}

void ServerControl::createServer()
{
    qDebug()<<"create server";
    this->server=new QTcpServer(this);
    this->server->listen(QHostAddress::AnyIPv4,11415);
    connect(server, &QTcpServer::newConnection, this, &ServerControl::newConnection);
    this->widget->getcreatbtn()->setEnabled(false);
}

void ServerControl::closeServer()
{
    delete this->server;
    widget->getcreatbtn()->setEnabled(true);
}

void ServerControl::newConnection()
{
    qDebug()<<"have new connection";
    while (this->server->hasPendingConnections()) {
        this->socket=server->nextPendingConnection();
        this->tcplist.append(this->socket);
        QString ip=this->socket->peerAddress().toString();
        this->widget->getlistwidget()->addItem(ip);
        connect(socket,&QTcpSocket::readyRead,this,&ServerControl::readMessage);
        connect(socket,&QTcpSocket::disconnected,this,&ServerControl::disConnection);
    }
}

void ServerControl::sendMessage(QTcpSocket *socket,QByteArray msg,QString client_tcptype)
{
    if(client_tcptype=="play")
    {
        const int maxBlockSize = 64*1024; // 32KB
        int totalBytes = msg.size();
        int bytesSent = 0;
        int sendnum=0;
        while (bytesSent<totalBytes)
        {
            sendnum=sendnum+1;
            int bytesToWrite = qMin(maxBlockSize,totalBytes-bytesSent);
            QByteArray block = msg.mid(bytesSent, bytesToWrite);
            qint64 bytesWritten = socket->write(block);
            bytesSent += bytesWritten;
            qDebug()<<"send msg to clicentsize:"<<block.size()<<"send num:"<<sendnum;
        }
    }
    else
    {
        socket->write(msg);
    }
    qDebug()<<"send msgsize is:"<<msg.size()<<"send to clicent";
}

void ServerControl::readMessage()
{
    QTcpSocket *socket=qobject_cast<QTcpSocket*>(sender());
    QByteArray buffer = socket->readAll();
    QString str(buffer);
    qDebug()<<"take user info:"<<str;
    for(int i=0;i<str.size();i++)
    {
        if(str[i]=="/")
        {
            this->client_tcptype=str.left(i);
            for(int j=i+1;j<str.size();j++)
            {
                if(str[j]=="/")
                {
                    this->client_userMsgtype=str.mid(i+1,j-i-1);
                    this->tcp_content=str.right(str.size()-j-1);
                    break;
                }
            }
            break;
        }
    }//将用户发送的请求进行分割处理
    qDebug()<<"client_tcptype is:"<<client_tcptype;//请求类型
    qDebug()<<"client_userMsgtype is:"<<client_userMsgtype;//请求内容：用户名字，乐曲名字等
    qDebug()<<"tcp_content is:"<<tcp_content;//请求处理数据内容：密码，乐曲id等
    if(client_tcptype=="find")
    {
        if(this->findlistdata==nullptr)
        {
            this->findlistdata=new FindList;
        }
        findlistdata->getFindinfo(this->client_userMsgtype,this->tcp_content);
        connect(findlistdata->filethread,&FileChangethread::filechanged,this,[=](QByteArray sendbuffer){
            QString str(sendbuffer);
            qDebug()<<"sendmsg in servercontol is"<<str;
            this->sendMessage(socket,sendbuffer,client_tcptype);
        });
    }//请求类型为查找对应检索乐曲列表清单
    else if(client_tcptype=="play")
    {
        if(this->playmusicdata==nullptr)
        {
            this->playmusicdata=new PlayMusic;
        }
        QByteArray sendmsg=this->playmusicdata->getPlayinfo(this->tcp_content);
        this->sendMessage(socket,sendmsg,client_tcptype);
    }//请求类型为获取指定单曲
    else if (client_tcptype=="login")
    {
        if(this->login==nullptr)
        {
             this->login=new LoginUser;
        }
        QByteArray loginstate=this->login->getLogininfo(client_userMsgtype,tcp_content);
        QString clicent(loginstate);
        qDebug()<<"send message is"<<clicent;
        QString str(buffer);
        qDebug()<<"server control take loginstate:"<<str;
        this->sendMessage(socket,loginstate,client_tcptype);
    }//请求类型为登录
    else if(client_tcptype=="adduser")
    {
        if(this->addnew==nullptr)
        {
            this->addnew=new AddUser;
        }
        QByteArray loginstate=this->addnew->getAdduserinfo(client_userMsgtype,tcp_content);
        QString clicent(loginstate);
        qDebug()<<"send message is"<<clicent;
        QString str(buffer);
        qDebug()<<"server control take loginstate:"<<str;
        this->sendMessage(socket,loginstate,client_tcptype);
    }//请求类型为增加用户
    else
    {
        qDebug()<<"this type error";
    }//未定义请求类型
}

void ServerControl::disConnection()
{
    QTcpSocket *socket=qobject_cast<QTcpSocket*>(sender());
    qDebug()<<"socket disconnection";
    this->tcplist.removeOne(socket);
    this->widget->getlistwidget()->clear();
    socket->deleteLater();
    for(QTcpSocket *sock:tcplist)
    {
        QString ip=sock->peerAddress().toString();
        this->widget->getlistwidget()->addItem(ip);
    }
}
