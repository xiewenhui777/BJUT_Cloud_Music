#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QDialog>
#include <QtWidgets/QMessageBox>
#include "chuanshu.h"
#include<QThread>
//register需要从主界面获取注册好的socket（用于信息传输）

registerDialog::registerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerDialog)
{
    ui->setupUi(this);
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("注册"));
    ui->pass->setEchoMode(QLineEdit::Password); //设置密码编辑框为隐藏模式
    ui->pass_->setEchoMode(QLineEdit::Password); //设置密码编辑框为隐藏模式
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    icon = "00";
    icoDia = new QDialog(this);
    layout_fri = new QVBoxLayout(icoDia);
    layout_fri->setMargin(20);
    layout_fri->setAlignment(Qt::AlignBaseline);
    //添加头像
    for(int i=0;i<7;i++)
    {
        QString path = QString(":/image/image/headicon/0%1.jpg").arg(i);            //注意文件夹的路径名称（与diamante中的资源保持一致）
        QPushButton *bu_temp = new QPushButton(icoDia);
        bu_temp->setIcon(QPixmap(path));
//        qDebug()<<"path is "<<path;
        icolist.append(bu_temp);
        layout_fri->addWidget(bu_temp);
        connect(bu_temp,&QPushButton::clicked,
                [=]() mutable
                {
                    this->icoDia->hide();
                    ui->headimage->setIcon(QPixmap(path));
                    //ui->messEdit->append("\n");
                    icon[0] = path[7];
                    icon[1] = path[8];
                }
                );
    }
    connect(ui->pushButton,SIGNAL(clicked()), this,SLOT(on_pushButton_clicked()));

}

registerDialog::~registerDialog()
{
    delete ui;
}



void registerDialog::on_pushButton_clicked()
{
    qDebug()<<"进入注册：";
    if(ui->name->text()!=""&&ui->gender->text()!=""&&ui->email->text()!=""&&ui->pass->text()!=""&&ui->pass_->text()!=""&&ui->userID->text()!=""&&ui->phone->text()!=""&&ui->pass->text()==ui->pass_->text())
    {
        qDebug()<<"注册成功";
        //信息完整且正确
        QString temp = QString("%1$%2$%3$%4$%5$%6")
                .arg(ui->userID->text()).arg(ui->name->text()).arg(ui->gender->text())
                .arg(ui->phone->text()).arg(ui->email->text())
                .arg(ui->pass->text());
        qDebug()<<"icon "<<icon;
        qDebug()<<"发给服务器的是： "<<temp;

        chuanshu *ss=new chuanshu("0######0#");
        QString s="";
        s+=ui->name->text()+"$";
        s+=ui->userID->text()+"$";
        s+=ui->gender->text()+"$";
        s+=ui->pass->text()+"$";
        s+=ui->phone->text()+"$";
        s+=ui->email->text()+"$";
        s+="";

        qDebug()<<"s:"<<s;

        ss->type = 16;
        ss->info = s;
        ss->timer = "";
        ss->name = ui->name->text();
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
        qDebug() <<"test send";


//        QThread::sleep(5000);       //进行等待休眠

//        QByteArray buffer;
//        //读取缓冲区数据
//        buffer = tcpSocket->readAll();
//        if(!buffer.isEmpty())
//        {
//            QString str;
//            //将收到的utf-8格式转换回String
//            str=QString::fromLocal8Bit(buffer.data());
//            qDebug()<<"rec:"<<str;
//        }

        QMessageBox::information(this, QStringLiteral("注册"), QStringLiteral("注册成功"));//显示注册成功信息的弹窗
    }else QMessageBox::information(this, QStringLiteral("注册"), QStringLiteral("注册失败"));

}

void registerDialog::closeEvent(QCloseEvent *event){
    state=1;
}
void registerDialog::regiSucce(QString info)
{
    //
    int i;
    QString id;
    for(i=0;i<info.size();i++)
    {
        if(info[i]==' ')
        {
            i++;
            break;
        }
    }
    for(int j=0;i<info.size();i++,j++)
    {
        id[j] = info[i];
    }
    qDebug()<<"服务器发来的id是 "<<id;
    QString temp = QString("系统给您分配的id是\n[%1]\n<请牢记您的id>").arg(id);
    QMessageBox::warning(this,QStringLiteral("id分配"),temp);
    this->hide();
}
void registerDialog::on_headimage_clicked()
{
    icoDia->show();
}


void registerDialog::socket_Read_Data()
{
    qDebug()<<"register";
    QByteArray buffer;
    //读取缓冲区数据
    buffer = tcpSocket->readAll();

    if(!buffer.isEmpty())
    {
        QString str;
        str=QString::fromLocal8Bit(buffer.data());//将收到的utf-8格式转换回String
        qDebug()<<"rec:"<<str;
//        ui->id->clear();
//        ui->id->setText(str);
    }
}

//int registerDialog::doExec()
//{
//    this->result = Rejected;
//    loop = new QEventLoop();
//    loop->exec();
//    return result;
//}


