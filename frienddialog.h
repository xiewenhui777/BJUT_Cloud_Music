#ifndef FRIENDDIALOG_H
#define FRIENDDIALOG_H

#include <QDialog>
#include "MusicList.h"
#include "MusicListWidget.h"
#include <QMenu>
#include "chat.h"

namespace Ui {
class FriendDialog;
}

class FriendDialog : public QDialog
{
    Q_OBJECT


private:
    Ui::FriendDialog *ui;
    QMenu *menu_list;
    chat chatdialog;        //聊天窗口
    QString friendlist;        //好友列表

public:
    explicit FriendDialog(QWidget *parent = nullptr);
    ~FriendDialog();

    void setFriend(QString list){
        friendlist=list;
    }
    void showlist();        //显示好友列表




private slots:
    void addFri();
    void chatfunction();
    void viewinfo();
    void on_friendlistWidget_customContextMenuRequested(const QPoint &pos);

};

#endif // FRIENDDIALOG_H
