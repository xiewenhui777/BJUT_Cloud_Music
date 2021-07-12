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

extern int quit_login;       //储存是否退出的状态
namespace Ui {
class PersonalDialog;
}

class PersonalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PersonalDialog(QWidget *parent = nullptr);
    ~PersonalDialog();
    feedbackDialog feedback;       //反馈
    FriendDialog fri;       //好友


private:
    Ui::PersonalDialog *ui;
    QDialog *icoDia1;
    QVBoxLayout *layout_fri1;
    QList <QPushButton *> icolist1;
    QString icon1;
    changePassword change1;
    information info1;

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
