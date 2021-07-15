#include "comment.h"
#include "ui_comment.h"
#include <QDebug>
#include "chuanshu.h"
#include <qdatetime.h>
#include <QMessageBox>
comment::comment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::comment)
{
    ui->setupUi(this);
    //ui->listWidget->setEnabled(false);           //不可编辑状态
    connect(ui->sendButton,SIGNAL(clicked()), this,SLOT(on_send_clicked()));
    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
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
    ss->info += "$"+content+"$"+current_date+"$"+musicname+"$"+userid;
    qDebug()<<ss->info;
    QString sender="";
    sender+=QString::number(ss->type)+"#"+(QString)ss->info+"#"+(QString)ss->timer+"#"+(QString)ss->name+"#"+(QString)ss->fileName+"#"+(QString)ss->wantsendto+"#"+QString::number(ss->size)+"#"+(QString)ss->ip;
//        // 发送
    char la=0xff;
    qDebug() <<sender.toUtf8();
    tcpSocket->write(sender.toUtf8()+la);
    tcpSocket->flush();
    this->showcomment();

}


void comment::on_fresh_clicked()
{
    ui->listWidget->clear();
    this->showcomment();
}

void comment::showcomment(){
    ui->listWidget->clear();
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

}
void comment::setrecv(QString rec){
    qDebug()<<rec;
    QString id="";
    QString use="";
    QStringList com=rec.split("|");
    qDebug()<<"number"<<com.length()<<endl;
    for(int i=0;i<com.length();i++){
        QStringList sincom=com[i].split("$");
        QString ut="user:"+sincom[4]+"   time:"+sincom[2]+"\n"+sincom[1];
        ui->listWidget->addItem(new QListWidgetItem(ut));
        id+=sincom[0]+"@@";
        use+=sincom[4]+"@@";
    }
    this->setcomid(id);
    this->setuser(use);
    qDebug()<<"id"<<id;

}



void comment::on_delete_2_clicked()
{
    QList<QListWidgetItem*> list = ui->listWidget->selectedItems();
    int r=-1;
     if(list.size() == 0)
      return;

     QListWidgetItem* sel = list[0];
     if (sel)
     {
      r = ui->listWidget->row(sel);
      qDebug()<<"number"<<r;
     }
     QStringList use=useid.split("@@");
     if(r!=-1&&userid==use[r]){
         ui->listWidget->takeItem(r);
         QStringList id=comid.split("@@");
         chuanshu *ss=new chuanshu("0######0#");
         qDebug()<<this->musicname;
         ss->type = 22;
         ss->info=id[r];
         QString sender="";
         sender+=QString::number(ss->type)+"#"+(QString)ss->info+"#"+"0"+"#"+"0"+"#"+"0"+"#"+"0"+"#"+"0"+"#"+"0";
               // 发送
         char la=0xff;
         qDebug() <<sender.toUtf8();
         tcpSocket->write(sender.toUtf8()+la);
         tcpSocket->flush();
         QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("评论删除成功"));
     }
     else{
         QMessageBox::information(this, QStringLiteral("错误"), QStringLiteral("非本人发送的评论"));
     }

}
