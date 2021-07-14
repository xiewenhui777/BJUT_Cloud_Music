#include "landwidget.h"
#include "ui_landwidget.h"
#include <QMessageBox>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QFile>
#include<QProcess>

LandWidget::LandWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LandWidget)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
//    setFixedSize(445,508);                     // 禁止拖动窗口大小
    setWindowTitle(QStringLiteral("登录"));    //设置窗口标题
    qDebug()<<"aa";
//    QObject::connect(tcpSocket, &QTcpSocket::readyRead, this, socket_Read_Data);
    //连接服务器

//    tcpSocket=new QTcpSocket();

//    tcpSocket->abort();
//    //连接服务器
//    tcpSocket->connectToHost("10.24.10.40", 8520);

//    //等待连接成功
//    if(!tcpSocket->waitForConnected(30000))
//    {
//        qDebug() << "Connection failed!";
//        return;
//    }
//    qDebug() << "Connect successfully!";

//    QObject::connect(tcpSocket, &QTcpSocket::readyRead, this, &LandWidget::socket_Read_Data);

//    char la=0xff;
//    QString send= "111";
//    qDebug() << "Send: " << send.toUtf8();

//    tcpSocket->write(send.toUtf8()+la);
//    tcpSocket->flush();
//    qDebug() <<tcpSocket;


//    ui->password->setEchoMode(QLineEdit::Password); //设置密码编辑框为隐藏模式
//    //判断是否已记住账号密码，如果是填充输入框
//    QFile file(":/data.txt");
//    file.open(QIODevice::ReadOnly | QIODevice::Text);
//    QByteArray temp = file.readAll();

    /*f(temp!=""){
        QString data = temp;
        int index_i = data.indexOf('_');
        int index_p = data.indexOf(';');
        QString id_re = data.mid(0,index_i);
        QString p_re = data.mid(index_i+1,index_p-index_i-1);
        qDebug()<<id_re<<" "<<p_re;
        ui->id->setText(id_re);
        ui->password->setText(p_re);

    }
    file.close();*/

    //注册按键
//    connect(ui->forgetpassword,SIGNAL(clicked()), this,SLOT(findPasswod_clicked()));
}


LandWidget::~LandWidget()
{
    delete ui;
}




void LandWidget::on_login_clicked()
{

    chuanshu *ss=new chuanshu("0######0#");         //先建立一个发送类
    QString s="";
    s+="$";
    s+=ui->id->text()+"$";
    s+="$";
    s+=ui->password->text()+"$";
    s+="$";
    s+="$";
    s+="";

    qDebug()<<"s:"<<s;

    ss->type = 15;
    ss->info = s;
    ss->timer = "";
    ss->name = ui->id->text();
    ss->fileName = "";
    ss->wantsendto = "";
    ss->size = 0;
    ss->ip = "";


    extern int quit_login;
    quit_login=0;           //退出状态重置
    this->hide();           //登录界面隐藏起来
    mainwidget = new MainWidget;
//    mainwidget->transfer(tcpSocket,ss->name);


    chuanshu *start=new chuanshu("0######0#");         //先建立一个发送类
    QString s1="";
    s1+="$";
    s1+=ui->id->text()+"$";
    s1+="$";
    s1+=ui->password->text()+"$";
    s1+="$";
    s1+="$";
    s1+="";

    qDebug()<<"s1:"<<s1;

    start->type = 0;
    start->info = s1;
    start->timer = "";
    start->name = ui->id->text();
    start->fileName = "";
    start->wantsendto = "";
    start->size = 0;
    start->ip = "10.24.6.228";


    QString sender1="";
    sender1+=QString::number(start->type)+"#"+(QString)start->info+"#"+(QString)start->timer+"#"+(QString)start->name+"#"+(QString)start->fileName+"#"+(QString)start->wantsendto+"#"+QString::number(start->size)+"#"+(QString)start->ip;


    mainwidget->show();         //启动主界面

    if(mainwidget->doExec() == MainWidget::Rejected){           //代表主界面的退出  因此result的状态会设置为rejected
        this->show();
        qDebug()<<"quit1"<<endl;

    }

}

//void LandWidget::on_registeButton_clicked()
//{
//    //点击注册按钮
////    QObject::disconnect(tcpSocket, &QTcpSocket::readyRead, this, &LandWidget::socket_Read_Data);
////    qDebug()<<"in regi";
//    regi = new Register();
//    regi->state=0;
//    regi->setSocket(this->tcpSocket);
//    regi->show();

////    if(regi->doExec() == Register::Rejected){           //代表主界面的退出  因此result的状态会设置为rejected
//////        this->show();
////        QObject::connect(tcpSocket, &QTcpSocket::readyRead, this, &LandWidget::socket_Read_Data);   //登录界面重新连接tcpSocket槽函数
////        qDebug()<<"quit Register page"<<endl;

////    }

////    MainWidget *test=new MainWidget;
////    test->doExec();

////    regi->doExec();


////     mainwidget = new MainWidget;
////     mainwidget->show();
////     mainwidget->doExec();


////    qDebug()<<"exit1";
////    QObject::connect(tcpSocket, &QTcpSocket::readyRead, this, &LandWidget::socket_Read_Data);
//}

//void LandWidget::findPasswod_clicked()        //找回密码的按钮
//{
//    forget1.show();
//    forget1.exec();
//}

//void LandWidget::socket_Read_Data()
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
