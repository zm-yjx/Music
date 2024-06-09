#include "filechangethread.h"

FileChangethread::FileChangethread(QString username,QString begindata)
{
    this->keydatastr=username+"/"+begindata;
    this->username=username;
}//初始化该线程时将对应用户名字与检索数据对本地文件进行操作

void FileChangethread::run()
{ 
    QString nowfile;
    while (1) {
        QFile fileIn("C:/Users/33746/Desktop/Find.txt");
        if (fileIn.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&fileIn);
            while (!in.atEnd())
            {
                nowfile = in.readLine();
            }
            if(this->keydatastr!=nowfile)
            {          
                QSqlDatabase::removeDatabase("qt_sql_default_connection");
                QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
                db.setHostName("localhost");
                db.setPort(3306);
                db.setDatabaseName("MUSIC"); //不是Database,记得别填错，应该是连接名字
                db.setUserName("root");
                db.setPassword("111111");
                bool ok = db.open();
                QString sendMsg;
                if (ok){
                    QString keyword=this->username;
                    QSqlQuery query(db);
                    QString sql=QString("select findlist_name,findlist_musicid from find_list where user_name='%1'").arg(keyword);
                    qDebug()<<sql;
                    bool ok1=query.exec(sql);
                    if(!ok1)
                    {
                        qDebug()<<query.lastError();
                        qDebug() << "获取数据失败"<<endl;
                        sendMsg="FINDERROR";
                    }
                    else
                    {
                        while (query.next())
                        {
                            qDebug() << query.value(0).toString() << query.value(1).toString();
                            sendMsg=sendMsg+query.value(0).toString()+";"+query.value(1).toString()+"/";
                        }
                    }
                    db.close();
                }
                else
                {
                    qDebug()<<"db error"<<db.lastError();
                    sendMsg="FINDERROR";
                }
                this->find=sendMsg.toUtf8();
                emit this->filechanged(this->find);//已经修改文件时在此处发送信号
                break;
            }
            fileIn.close();
        }
        else
        {
            qDebug() << "Failed to open file for reading.";
        }
        QThread::sleep(1);
    }//通过死循环来持续检测文件内容是否改变，因为定义规则为如果爬虫端数据处理完毕将修改对应文本的内容，也就以为这已经拿到了需要的歌单数据
}
