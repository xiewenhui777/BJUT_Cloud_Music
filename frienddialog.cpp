#include "frienddialog.h"
#include "ui_frienddialog.h"
#include "frilist.h"
#include "myqss.h"          //样式的头文件
#include <QScrollBar>
//#include "MusicListDialog.h"

FriendDialog::FriendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FriendDialog)
{
    ui->setupUi(this);
    ui->friendlistWidget->verticalScrollBar()->setStyleSheet(ListWidgetStyle());
    ui->friendlistWidget->setIcon(QIcon(":/image/image/image/friend.png"));
    connect(ui->addfriend,SIGNAL(clicked()), this,SLOT(addFri()));      //添加好友
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
