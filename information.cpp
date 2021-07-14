#include "information.h"
#include "ui_information.h"

information::information(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::information)
{
    ui->setupUi(this);
    ui->email->setEnabled(false);
    ui->userID->setEnabled(false);
    ui->phone->setEnabled(false);
    ui->gender->setEnabled(false);
    ui->name->setEnabled(false);

    connect(ui->edit,SIGNAL(clicked()), this,SLOT(edit_clicked()));
    connect(ui->save,SIGNAL(clicked()), this,SLOT(save_clicked()));
}

information::~information()
{
    delete ui;
}


void information::edit_clicked(){
    ui->email->setEnabled(true);
//    ui->userID->setEnabled(true);
    ui->phone->setEnabled(true);
    ui->gender->setEnabled(true);
    ui->name->setEnabled(true);
//    close();
//    exec();     //进程结束
}
void information::save_clicked(){
    chuanshu *ss=new chuanshu("0######0#");
    QString s="";
    s+=ui->name->text()+"$";
    s+=ui->userID->text()+"$";
    s+=ui->gender->text()+"$";
    s+="$";
    s+=ui->phone->text()+"$";
    s+=ui->email->text()+"$";
    s+="";

    qDebug()<<"register info:"<<s;

    ss->type = 11;
    ss->info = s;
    ss->timer = "";
    ss->name = ui->userID->text();
    ss->fileName = "";
    ss->wantsendto = "";
    ss->size = 0;
    ss->ip = "";

    QString sender="";
    sender+=QString::number(ss->type)+"#"+(QString)ss->info+"#"+(QString)ss->timer+"#"+(QString)ss->name+"#"+(QString)ss->fileName+"#"+(QString)ss->wantsendto+"#"+QString::number(ss->size)+"#"+(QString)ss->ip;

    // 发送
    char la=0xff;
    qDebug() <<sender.toUtf8();
    tcpSocket->write(sender.toUtf8()+la);
    tcpSocket->flush();
    qDebug() <<"change info send";
    ui->email->setEnabled(false);
    ui->userID->setEnabled(false);
    ui->phone->setEnabled(false);
    ui->gender->setEnabled(false);
    ui->name->setEnabled(false);
    close();
    exec();     //进程结束
}
void information::info_show(){      //设置个人信息显示
    ui->userID->setText(info_userID);
    ui->name->setText(info_username);
    ui->gender->setText(info_usergender);
    ui->phone->setText(info_usertel);
    ui->email->setText(info_useremail);
}
