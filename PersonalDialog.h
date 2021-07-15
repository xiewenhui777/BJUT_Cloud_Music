#ifndef PERSONALDIALOG_H
#define PERSONALDIALOG_H


#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include "changepassword.h"
#include "information.h"
#include "feedbackdialog.h"
#include "frienddialog.h"
#include "chuanshu.h"
#include <QTcpSocket>

#include  <QDebug>
extern int quit_login;       //储存是否退出的状态
namespace Ui {
class PersonalDialog;
}

class PersonalDialog : public QDialog
{
    Q_OBJECT
private:
    Ui::PersonalDialog *ui;
    QDialog *icoDia1;
    QVBoxLayout *layout_fri1;
    QList <QPushButton *> icolist1;
    QString icon1;
    changePassword change1;
    information info1;
    QString username;   //用户名
        QString userpassword;       //用户密码（可用于修改密码时进行对比）
        QString userID;     //用户账号
        QString usertel;
        QString useremail;
        QString friendinfo;     //好友信息（包括在线状态）
        QString usergender;
        QTcpSocket *tcpSocket;
     QString friendlist;        //好友列表
public:
    explicit PersonalDialog(QWidget *parent = nullptr);
    ~PersonalDialog();
    feedbackDialog feedback;       //反馈
    FriendDialog fri;       //好友
    void setUsername(QString name){
            username=name;
        }
        void setUserpassword(QString password){
            userpassword=password;
        }
        void setUserID(QString id){
            userID=id;
        }
        void setTel(QString tel){
            usertel=tel;
        }
        void setEmail(QString email){
            useremail=email;
        }
        void setFriendinfo(QString friendinfo1){
            friendinfo=friendinfo1;
        }
        void setGender(QString gender){
            usergender=gender;
        }

        void showInfo();

        void setSocket(QTcpSocket *soc)
        {
            tcpSocket = soc;
        }

        void setFriend(QString list){
            friendlist=list;
        }


private slots:          //响应时间需要单独加一个slot!!!
    void on_exit_clicked();
    void on_headimage_clicked();
    void changePasswod_clicked();
    void info_clicked();
    void on_feedback_clicked();
    void on_friend_clicked();
//    LandWidget * land;
};

#endif // PERSONALDIALOG_H
