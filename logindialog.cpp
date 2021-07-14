#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QFile>
#include<QProcess>

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮               // 禁止拖动窗口大小
    setWindowTitle(QStringLiteral("登录"));    //设置窗口标题
//    connect(ui->forgetpassword,SIGNAL(clicked()), this,SLOT(findPasswod_clicked()));

}

loginDialog::~loginDialog()
{
    delete ui;
}


//void loginDialog::on_login_clicked()
//{
//    chuanshu *ss=new chuanshu("0######0#");         //先建立一个发送类
//    QString s="";
//    s+="$";
//    s+=ui->id->text()+"$";
//    s+="$";
//    s+=ui->password->text()+"$";
//    s+="$";
//    s+="$";
//    s+="";

//    qDebug()<<"s:"<<s;

//    ss->type = 15;
//    ss->info = s;
//    ss->timer = "";
//    ss->name = ui->id->text();
//    ss->fileName = "";
//    ss->wantsendto = "";
//    ss->size = 0;
//    ss->ip = "";



////    extern int quit_login;
////    quit_login=0;           //退出状态重置
////    this->hide();           //登录界面隐藏起来
////    mainwidget = new MainWidget;


//    chuanshu *start=new chuanshu("0######0#");         //先建立一个发送类
//    QString s1="";
//    s1+="$";
//    s1+=ui->id->text()+"$";
//    s1+="$";
//    s1+=ui->password->text()+"$";
//    s1+="$";
//    s1+="$";
//    s1+="";

//    qDebug()<<"s1:"<<s1;

//    start->type = 0;
//    start->info = s1;
//    start->timer = "";
//    start->name = ui->id->text();
//    start->fileName = "";
//    start->wantsendto = "";
//    start->size = 0;
//    start->ip = "10.24.6.228";


//    QString sender1="";
//    sender1+=QString::number(start->type)+"#"+(QString)start->info+"#"+(QString)start->timer+"#"+(QString)start->name+"#"+(QString)start->fileName+"#"+(QString)start->wantsendto+"#"+QString::number(start->size)+"#"+(QString)start->ip;
//    exec();     //登录进程结束
//    close();

//}


//void loginDialog::on_registeButton_clicked()
//{
//    //点击注册按钮
////    QObject::disconnect(tcpSocket, &QTcpSocket::readyRead, this, &loginDialog::socket_Read_Data);
////    qDebug()<<"in regi";
//    regi = new Register();
//    regi->state=0;
//    regi->setSocket(this->tcpSocket);
//    regi->show();

//}

//void loginDialog::findPasswod_clicked()        //找回密码的按钮
//{
//    qDebug()<<"找回密码";
//    forget1.show();
//    forget1.exec();
//}

//void loginDialog::socket_Read_Data()
//{
//    qDebug()<<"socket_Read_Data";
//    QByteArray buffer;
//    //读取缓冲区数据
//    buffer = tcpSocket->readAll();

//    if(!buffer.isEmpty())
//    {
//        str=QString::fromLocal8Bit(buffer.data());//将收到的utf-8格式转换回String
//        qDebug()<<"rec:"<<str;
////        ui->id->clear();
////        ui->id->setText(str);
//    }

//    QStringList sstr=str.split("#");
//    qDebug()<<"type:"<<sstr[0].toInt();


//    if(sstr[0].toInt()==3){        //判断回传包的类型
//        qDebug()<<"s1";
//        QStringList arguments;//用于传参数
//        QString program = "D:\\Transfer\\receive.exe"; //外部程序地址
//        arguments <<"10.24.10.40"<<"8888";
//        qDebug()<<"s2";
//        QProcess process(this);
//        process.startDetached(program, arguments);//启动程序
//        process.close();
//        qDebug()<<"s3";
//    }
//}
