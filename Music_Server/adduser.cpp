#include "adduser.h"

AddUser::AddUser(QObject *parent) : QObject(parent)
{

}

QByteArray AddUser::getAdduserinfo(QString user_name, QString passwwd)
{
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("MUSIC"); //不是Database,记得别填错，应该是连接名字
    db.setUserName("root");
    db.setPassword("111111");
    bool ok = db.open();
    QString sendMsg="";
    if (ok){
        QString keyword=user_name;
        QString keyword2=passwwd;
        QSqlQuery query(db);
        QString sql=QString("insert into user_info(user_name,user_passwd) values('%1','%2')").arg(keyword,keyword2);
        qDebug()<<sql;
        bool ok1=query.exec(sql);
        if(!ok1)
        {
                qDebug()<<query.lastError();
                qDebug() << "获取数据失败"<<endl;
                sendMsg="ADDERROR";
        }
        else
        {
            sendMsg="ADDUSER";
        }
        db.close();
    }
    else
    {
        qDebug()<<"db error"<<db.lastError();
        sendMsg="ADDERROR";
    }
    QByteArray buffer;
    buffer=sendMsg.toUtf8();
    return buffer;
}//通过该方法向user表中插入数据，当用户是第一次注册时使用
