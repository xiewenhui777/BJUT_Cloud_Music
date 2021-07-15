#include "frienddialog.h"
#include "ui_frienddialog.h"
#include "frilist.h"
#include "myqss.h"          //样式的头文件
#include <QScrollBar>
#include <QMessageBox>
#include "chuanshu.h"
//#include "MusicListDialog.h"

FriendDialog::FriendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FriendDialog)
{
    ui->setupUi(this);
    ui->friendlistWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());
    ui->friendlistWidget->setIcon(QIcon(":/image/image/image/friend.png"));
    ui->songWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());
    ui->songWidget->setIcon(QIcon(":/image/image/image/dialog-music.png"));



    ui->friendlistWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_frilist_chat=new QAction(QIcon(":/image/image/image/chat.png"),u8"聊天");
    connect(action_frilist_chat,&QAction::triggered,this,&FriendDialog::chatfunction);
    QAction *action_frilist_info=new QAction(QIcon(":/image/image/image/ziliao.png"),u8"查看好友歌单");
    connect(action_frilist_info,&QAction::triggered,this,&FriendDialog::viewsong);
    menu_list=new QMenu(this);
    menu_list->addAction(action_frilist_chat);
    menu_list->addAction(action_frilist_info);


    ui->songWidget->setContextMenuPolicy(Qt::CustomContextMenu);
//    QAction *action_song=new QAction(QIcon(":/image/image/image/To-playlist.png"),u8"查看歌曲");
//    connect(action_song,&QAction::triggered,this,&FriendDialog::viewquzi);
//    menu_list1=new QMenu(this);
//    menu_list1->addAction(action_song);


    connect(ui->addfriend,SIGNAL(clicked()), this,SLOT(addFri()));      //添加好友槽函数
    connect(ui->songfresh,SIGNAL(clicked()), this,SLOT(showsong()));
    connect(ui->quzi,SIGNAL(clicked()), this,SLOT(viewquzi()));

}

FriendDialog::~FriendDialog()
{
    delete ui;
}


void FriendDialog::addFri(){
    FriList *fr = new FriList(this);
    fr->addFriend(10001,"kid","00","hahah");
    fr->addFriend(10002,"赵满刚","03","呵呵呵呵");
    //首先建立一个music好友类
    Music testMusic;
    testMusic.url="abcd";
    testMusic.title="for test";
    testMusic.author="xwh";
    testMusic.duration=120;
    testMusic.albumTitle="test case";
    testMusic.audioBitRate=32000;
    ui->friendlistWidget->musicList.addMusic(testMusic);        //添加歌曲
    ui->friendlistWidget->refresh();
}

void FriendDialog::on_friendlistWidget_customContextMenuRequested(const QPoint &pos)        //右键菜单显示
{
    qDebug()<<"up";
    if(ui->friendlistWidget->itemAt(pos)==Q_NULLPTR)
    {
        return;
    }
    menu_list->exec(QCursor::pos());
}


void FriendDialog::on_songlistWidget_customContextMenuRequested(const QPoint &pos)        //右键菜单显示
{
    qDebug()<<"down";
    if(ui->songWidget->itemAt(pos)==Q_NULLPTR)
    {
        return;
    }
    menu_list1->exec(QCursor::pos());
}
void FriendDialog::chatfunction(){      //进入某个好友的聊天界面
    //进入好友界面前进行信息设置   首先得从widget中获取位置后  在进行名字的获取
    int pos=ui->friendlistWidget->currentRow();     //获取点击位置
    Music friendinfo=ui->friendlistWidget->musicList.getMusic(pos);
    chatdialog.setName(friendinfo.title);
    chatdialog.showInfo();

    chatdialog.show();
    chatdialog.exec();
    QMessageBox::information(this, QStringLiteral("聊天"), QStringLiteral("聊天成功"));//显示注册成功信息的弹窗
}

void FriendDialog::viewsong(){      //查看好友歌单            //发送消息到服务器中去
    int pos=ui->friendlistWidget->currentRow();     //获取点击位置
    Music friendinfo=ui->friendlistWidget->musicList.getMusic(pos);

    chuanshu *ss=new chuanshu("0######0#");         //先建立一个发送类
    QString s=friendinfo.author;       //放置好友ID
    friID=friendinfo.author;

    qDebug()<<"view friend id:"<<s;

    ss->type = 30;
    ss->info = s;
    ss->timer = "";
    ss->name = userID;
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
    qDebug() <<"view song sned over";
}


void FriendDialog::showlist(){      //好友列表显示
    //显示之前都需要进行列表清理
    ui->friendlistWidget->musicList.clear();
    ui->friendlistWidget->clear();
        QStringList Friendinfo=friendlist.split("|");           //切分出好友
    for(int i=0;i<Friendinfo.size()-1;i++){     //循环添加上去        -1是需要把自己给删除掉
        QString info1=Friendinfo[i];           //获取第i个好友信息
        qDebug()<<"each info1:"<<info1;          //打印出每个好友的信息
        QStringList info=info1.split("$");     //切分出用户信息出来

        Music testMusic;
        testMusic.url=info[2];      //设置为在线状态
        testMusic.title=info[1];     //设置为用户的名字
        testMusic.author=info[0];       //设置为用户的ID
        testMusic.duration=0;       //无用
        testMusic.albumTitle="Friend";
        testMusic.audioBitRate=8888;            //可以随便设置
        ui->friendlistWidget->musicList.addMusic(testMusic);        //添加歌曲
    }
    ui->friendlistWidget->refresh();        //添加完成后进行显示
}

void FriendDialog::showsong(){      //刷新显示歌单列表
//    qDebug()<<"hhh";
    ui->songWidget->musicList.clear();
    ui->songWidget->clear();

    QStringList Friendinfo=Songlist.split("$");           //切分出好友
    for(int i=0;i<Friendinfo.size();i++){     //循环添加上去
        QString info1=Friendinfo[i];           //获取第i个好友信息
        qDebug()<<"each songlistname:"<<info1;          //

        Music testMusic;
        testMusic.url="fake url";      //
        testMusic.title=info1;     //设置歌单名称
        testMusic.author=friID;       //
        testMusic.duration=0;       //无用
        testMusic.albumTitle="FriendSongList";
        testMusic.audioBitRate=8888;            //可以随便设置

        ui->songWidget->musicList.addMusic(testMusic);        //添加歌曲
    }
    ui->songWidget->refresh();        //添加完成后进行显示
}

void FriendDialog::setSonglist(QString list){
    Songlist=list;
    qDebug()<<"friend songlist:"<<Songlist;
}

void FriendDialog::viewquzi(){

    int pos=ui->songWidget->currentRow();     //获取点击位置
    Music friendinfo=ui->songWidget->musicList.getMusic(pos);
    qDebug()<<"pos:"<<pos;
    chuanshu *ss=new chuanshu("0######0#");         //先建立一个发送类
    QString s=friendinfo.title;       //放置好友ID

    ss->type = 31;
    ss->info = s;
    ss->timer = "";
    ss->name = friendinfo.author;
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
    qDebug() <<"view quzi sned over";
}

