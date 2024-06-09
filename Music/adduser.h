#ifndef ADDUSER_H
#define ADDUSER_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QTcpSocket>

namespace Ui {
class AddUser;
}

class AddUser : public QWidget
{
    Q_OBJECT
public:
    explicit AddUser(QWidget *parent = nullptr);
    QString getnewusername();
    QString getpasswd();
    void setAddsocket(QTcpSocket * keysocket);
    ~AddUser();
private slots:
    void on_pushButton_clicked();
private:
    QString username;
    QString userpasswd;
    QTcpSocket *addsocket;
    Ui::AddUser *ui;
signals:
    void senduserinfo(QString username,QString userpasswd);
};

#endif // ADDUSER_H
