#include "frienddialog.h"
#include "ui_frienddialog.h"
#include "frilist.h"
#include "myqss.h"          //样式的头文件
#include <QScrollBar>
#include <QMessageBox>
//#include "MusicListDialog.h"

FriendDialog::FriendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FriendDialog)
{
    ui->setupUi(this);
    ui->friendlistWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());
    ui->friendlistWidget->setIcon(QIcon(":/image/image/image/friend.png"));

    ui->friendlistWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *action_frilist_chat=new QAction(QIcon(":/image/image/image/chat.png"),u8"聊天");
    connect(action_frilist_chat,&QAction::triggered,this,&FriendDialog::chatfunction);
    QAction *action_frilist_info=new QAction(QIcon(":/image/image/image/ziliao.png"),u8"查看好友歌单");
    connect(action_frilist_info,&QAction::triggered,this,&FriendDialog::viewinfo);

    menu_list=new QMenu(this);
    menu_list->addAction(action_frilist_chat);
    menu_list->addAction(action_frilist_info);

    connect(ui->addfriend,SIGNAL(clicked()), this,SLOT(addFri()));      //添加好友槽函数
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
    if(ui->friendlistWidget->itemAt(pos)==Q_NULLPTR)
    {
        return;
    }
    menu_list->exec(QCursor::pos());
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

void FriendDialog::viewinfo(){      //查看好友歌单

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


