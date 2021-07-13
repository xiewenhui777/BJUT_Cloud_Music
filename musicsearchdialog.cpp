#include "MusicSearchDialog.h"
#include "ui_MusicSearchDialog.h"
#include <QtWidgets/QMessageBox>
#include <QVBoxLayout>
#include "chuanshu.h"
#include <QProcess>

MusicSearchDialog::MusicSearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MusicSearchDialog)
{
    ui->setupUi(this);
    ui->SearchlineEdit->clear();
    ui->SearchButton->setCursor(Qt::PointingHandCursor);
    ui->SearchButton->setFixedSize(22, 22);
    ui->SearchButton->setToolTip(QStringLiteral("搜索"));
    ui->SearchButton->setStyleSheet("QPushButton{border-image:url(:/images/icon_search_normal); background:transparent;} \
                                         QPushButton:hover{border-image:url(:/images/icon_search_hover)} \
                                         QPushButton:pressed{border-image:url(:/images/icon_search_press)}");

    //防止文本框输入内容位于按钮之下
    QMargins margins = ui->SearchlineEdit->textMargins();
    ui->SearchlineEdit->setTextMargins(margins.left(), margins.top(), ui->SearchButton->width(), margins.bottom());
    ui->SearchlineEdit->setPlaceholderText(QStringLiteral("请输入搜索内容"));

    QHBoxLayout *pSearchLayout = new QHBoxLayout();
    pSearchLayout->addStretch();
    pSearchLayout->addWidget(ui->SearchButton);
    pSearchLayout->setSpacing(0);
    pSearchLayout->setContentsMargins(0, 0, 0, 0);
    ui->SearchlineEdit->setLayout(pSearchLayout);

    connect(ui->SearchButton, SIGNAL(clicked(bool)), this, SLOT(search()));
    connect(ui->downloadButton, SIGNAL(clicked(bool)), this, SLOT(on_download_clicked()));
    QObject::connect(tcpSocket, &QTcpSocket::readyRead, this, &MusicSearchDialog::socket_Read_Data);

}

MusicSearchDialog::~MusicSearchDialog()
{ 
    ui->SearchlineEdit->clear();
    delete ui;
}

void MusicSearchDialog::search(){
    QString strText = ui->SearchlineEdit->text();
    //若在数据库中搜寻到某个歌曲 则显将结果显示在搜索弹框中
    if (!strText.isEmpty())             //此处应该修改为当搜索不到歌曲时  则显示“无当前搜索歌曲”（这得结合数据库去实现）
    {

//        chuanshu *ss=new chuanshu("0######0#");
//        ss->type = 17;
//        ss->info = ui->SearchlineEdit->text();
//        ss->timer = "";
//        ss->name = userID;
//        ss->fileName = "";
//        ss->wantsendto = "";
//        ss->size = 0;
//        ss->ip = "";

//        QString sender="";
//        sender+=QString::number(ss->type)+"#"+(QString)ss->info+"#"+(QString)ss->timer+"#"+(QString)ss->name+"#"+(QString)ss->fileName+"#"+(QString)ss->wantsendto+"#"+QString::number(ss->size)+"#"+(QString)ss->ip;

//        // 发送
//        char la=0xff;
////        qDebug() <<sender.toUtf8();
//        tcpSocket->write(sender.toUtf8()+la);
//        tcpSocket->flush();
////        qDebug() <<"search send";


        QMessageBox::information(this, QStringLiteral("搜索"), QStringLiteral("搜索内容为%1").arg(strText));
    }
}


void MusicSearchDialog::socket_Read_Data()
{
    qDebug()<<"xiazai:";
    QByteArray buffer;
    //读取缓冲区数据
    buffer = tcpSocket->readAll();
    QString str;
    if(!buffer.isEmpty())
    {

        str=QString::fromLocal8Bit(buffer.data());//将收到的utf-8格式转换回String
        qDebug()<<"search rec:"<<str;
    }

    QStringList sstr=str.split("#");
    qDebug()<<"type:"<<sstr[0].toInt();


    if(sstr[0].toInt()==3){        //判断回传包的类型
        QStringList arguments;//用于传参数
        QString program = "D:\\Transfer\\receive.exe"; //外部程序地址
        arguments <<"10.24.10.40"<<"8888";
        QProcess process(this);
        process.startDetached(program, arguments);//启动程序
        process.close();
    }
}

void MusicSearchDialog::on_download_clicked(){      //点击下载音乐

    chuanshu *ss=new chuanshu("0######0#");
    ss->type = 3;
    ss->info = ui->SearchlineEdit->text();
    ss->timer = "";
    ss->name = userID;
    ss->fileName = "斯卡布罗集市";
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
    qDebug() <<"search send";

}
