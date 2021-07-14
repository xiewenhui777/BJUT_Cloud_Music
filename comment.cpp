#include "comment.h"
#include "ui_comment.h"
#include <QDebug>
#include "chuanshu.h"
#include <qdatetime.h>
comment::comment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::comment)
{
    ui->setupUi(this);
    ui->messBrowser->setEnabled(false);           //不可编辑状态
    connect(ui->sendButton,SIGNAL(clicked()), this,SLOT(on_send_clicked()));

}

comment::~comment()
{
    delete ui;
}



void comment::on_send_clicked(){        //发送文本框中的内容
    QString content=ui->messEdit->toPlainText();      //获取文本框总的内容

    qDebug()<<content;
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<current_date;
    ui->messEdit->clear();
    chuanshu *ss=new chuanshu("0######0#");
    qDebug()<<this->musicname;
    ss->type = 21;
    ss->info="1";
    ss->info += "$"+content+"$"+current_date+"$"+musicname+"$"+"1";
    qDebug()<<ss->info;
    QString sender="";
    sender+=QString::number(ss->type)+"#"+(QString)ss->info+"#"+(QString)ss->timer+"#"+(QString)ss->name+"#"+(QString)ss->fileName+"#"+(QString)ss->wantsendto+"#"+QString::number(ss->size)+"#"+(QString)ss->ip;
//        // 发送
    char la=0xff;
    qDebug() <<sender.toUtf8();
    tcpSocket->write(sender.toUtf8()+la);
    tcpSocket->flush();

}


void comment::on_fresh_clicked()
{
    ui->messBrowser->clear();
    this->showcomment();
}

void comment::showcomment(){
    ui->label_3->setText(musicname);
    chuanshu *ss=new chuanshu("0######0#");
    qDebug()<<this->musicname;
    ss->type = 19;
    ss->info=musicname;
    qDebug()<<ss->info;
    QString sender="";
    sender+=QString::number(ss->type)+"#"+(QString)ss->info+"#"+"0"+"#"+"0"+"#"+"0"+"#"+"0"+"#"+"0"+"#"+"0";
          // 发送
    char la=0xff;
    qDebug() <<sender.toUtf8();
    tcpSocket->write(sender.toUtf8()+la);
    tcpSocket->flush();
    //while(recoment=="");

}
void comment::setrecv(QString rec){
    qDebug()<<rec;
    QStringList com=rec.split("|");
    for(int i=0;i<com.length();i++){
        QStringList sincom=com[i].split("$");
        QString ut="user:"+sincom[0]+"   time:"+sincom[2];
        ui->messBrowser->append(ut);
        ui->messBrowser->append(sincom[1]);
        ui->messBrowser->append("------------------------------------------------------------");
    }

}


