#ifndef FRIENDDIALOG_H
#define FRIENDDIALOG_H

#include <QDialog>
#include "MusicList.h"
#include "MusicListWidget.h"
#include <QMenu>
#include "chat.h"
#include <QTcpSocket>

namespace Ui {
class FriendDialog;
}

class FriendDialog : public QDialog
{
    Q_OBJECT


private:
    Ui::FriendDialog *ui;
    QMenu *menu_list;
    QMenu *menu_list1;
    chat chatdialog;        //聊天窗口
    QString friendlist;        //好友列表
    QString songlist;       //歌单列表
    QString userID;
    QTcpSocket *tcpSocket;
    QString Songlist;
    vector<MusicList> musiclist1;
    QString friID;      //好友ID

public:
    explicit FriendDialog(QWidget *parent = nullptr);
    ~FriendDialog();

    void setFriend(QString list){
        friendlist=list;
    }
    void showlist();        //显示好友列表
    void setSong(QString song){
        songlist=song;
    }

    void setUserid(QString id){
        userID=id;
    }
    void setSocket(QTcpSocket *soc)
    {
        tcpSocket = soc;
    }
    void setSonglist(QString list);




private slots:
    void addFri();
    void chatfunction();
    void viewsong();
    void on_friendlistWidget_customContextMenuRequested(const QPoint &pos);
    void on_songlistWidget_customContextMenuRequested(const QPoint &pos);       //右键菜单显示
    void showsong();        //刷新显示好友歌单
    void viewquzi();        //查看好友歌曲
//    void viewsong1();

};

#endif // FRIENDDIALOG_H
