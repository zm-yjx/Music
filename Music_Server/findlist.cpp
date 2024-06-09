#include "findlist.h"

FindList::FindList()
{

}

void FindList::getFindinfo(QString user_name,QString findmsg)
{
    this->setFindMsg(user_name,findmsg);
    this->filethread=new FileChangethread(user_name,findmsg);
    this->filethread->start();//当需要检索具体歌单的时候启动子线程，防止服务器被卡死
}

void FindList::setFindMsg(QString username,QString msg)
{
    QString buffer=username+"/"+msg;
    QFile file("C:/Users/33746/Desktop/Find.txt");//打开存放有搜索内容文本文件
    file.open(QIODevice::WriteOnly | QIODevice::Text);//采取写模式实现往搜索内容文档中写入内容
    QTextStream out(&file);
    out<<buffer;
    file.close();//写入数据
}//将检索数据写入到对应文件内
