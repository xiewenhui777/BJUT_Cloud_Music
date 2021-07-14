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
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮                    // 禁止拖动窗口大小
    setWindowTitle(QStringLiteral("登录"));    //设置窗口标题
    qDebug()<<"aa";

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


