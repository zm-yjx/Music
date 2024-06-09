#include "loginuser.h"

LoginUser::LoginUser(QObject *parent) : QObject(parent)
{

}

QByteArray LoginUser::getLogininfo(QString user_name,QString passwwd)
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
        QSqlQuery query(db);
        QString sql=QString("select * from user_info where user_name='%1'").arg(keyword);
        qDebug()<<sql;
        bool ok1=query.exec(sql);
        if(!ok1)
        {
                qDebug() << "获取数据失败"<<endl;
        }
        else
        {
            //取出当前登录记录中的字段
            if(query.first())
            {
                if(passwwd==query.value(1).toString())
                {
                   qDebug()<<"passwd is"<<query.value(1).toString();
                   sendMsg="LOGIN";
                }
                else {
                   sendMsg="ERROR";
                }
            }
        }
        db.close();
    }
    else
    {
        qDebug()<<"db error"<<db.lastError();
    }
    QByteArray buffer;
    buffer=sendMsg.toUtf8();
    return buffer;
}//需要对应数据库内指定用户姓名进行操作，即查找用户姓名与对应密码是否对应
