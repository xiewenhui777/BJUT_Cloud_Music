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
    QAction *action_frilist_info=new QAction(QIcon(":/image/image/image/ziliao.png"),u8"查看资料");
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
    //首先建立一个music类
    Music testMusic;
    testMusic.url="abcd";
    testMusic.title="for test";
    testMusic.author="xwh";
    testMusic.duration=120;
    testMusic.albumTitle="test case";
    testMusic.audioBitRate=32000;
    ui->friendlistWidget->musicList.addMusic(testMusic);        //添加歌曲
//    ui->searchWidget->musicList.addMusic(musiclist[musiclist_index].getMusic(pos));
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
    QMessageBox::information(this, QStringLiteral("聊天"), QStringLiteral("聊天成功"));//显示注册成功信息的弹窗
}

void FriendDialog::viewinfo(){      //查看好友资料

}
