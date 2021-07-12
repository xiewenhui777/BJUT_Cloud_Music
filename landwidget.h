#ifndef LANDWIDGET_H
#define LANDWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include "register.h"
#include "mainWidget.h"
#include "forgetpassword.h"
#include "chuanshu.h"

namespace Ui {
class LandWidget;
}

class LandWidget : public QWidget
{
    Q_OBJECT

public:
    int user_active;    //当前用户
    explicit LandWidget(QWidget *parent = nullptr);
    ~LandWidget();
//    void setTcpSocket(QTcpSocket *tcp); //设置通信套接字
    QString landPack(int id,QString password); //打包登陆信息
    bool judgement(QByteArray array);    //判断服务器返回的数据
public slots:
//    void slotDataRecv();    //处理收到的服务器消息

signals:
    void landSuccess(); //登陆成功信号


private slots:
    void on_login_clicked();
    void on_registeButton_clicked();
    void findPasswod_clicked();
//    void socket_Read_Data();


private:
    Ui::LandWidget *ui;
    QTcpSocket *tcpSocket;
    Register *regi;
    MainWidget *mainwidget;      //主界面的实例
    forgetPassword forget1;
    QString str;

};

#endif // LANDWIDGET_H
