#include "playmusic.h"

PlayMusic::PlayMusic()
{

}

QByteArray PlayMusic::getPlayinfo(QString music_id)
{
    qDebug()<<"begin take musicdata";
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("MUSIC"); //不是Database,记得别填错，应该是连接名字
    db.setUserName("root");
    db.setPassword("111111");
    qDebug()<<"login sql database";
    bool ok = db.open();
    QByteArray buffer;
    if (ok){
        QString keyword=music_id;
        QSqlQuery query(db);
        QString sql=QString("select music_path from musicinfo_list where findlist_musicid='%1'").arg(keyword);
        qDebug()<<sql;
        bool ok1=query.exec(sql);
        if(!ok1)
        {
                qDebug() << "操作数据库失败"<<endl;
        }
        else
        {
            //取出当前记录中的字段
            while (query.next()) {
                qDebug()<<query.value(0).toString();
                QUrl musicurl(query.value(0).toString());
                // 读取本地音乐文件
                QFile file(query.value(0).toString());
                if(!file.open(QIODevice::ReadOnly)) {
                    qDebug() << "Failed to open file:" << file.errorString();
                }
                else
                {
                    buffer = file.readAll();
                    file.close();
                }
            }//通过地址拿到对应的单曲文件，将单曲文件转换为对应的字节数组
        }
        db.close();
    }
    else
    {
        qDebug()<<"db error:";
    }
    return buffer;
}//将数据库存储地址拿到对应文件后需要将该音乐数据转化为字节数组
