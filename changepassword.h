#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>
#include "chuanshu.h"
#include <QTcpSocket>

namespace Ui {
class changePassword;
}

class changePassword : public QDialog
{
    Q_OBJECT

public:
    explicit changePassword(QWidget *parent = nullptr);
    ~changePassword();
    void setpassword(QString password){
        userpassword=password;
    }
    void setSocket(QTcpSocket *soc)
    {
        tcpSocket = soc;
    }
    void setname(QString name){
        info_username=name;
    }
    void setuserID(QString ID){
        info_userID=ID;
    }
    void setgender(QString gender){
        info_usergender=gender;
    }
    void setemail(QString email){
        info_useremail=email;
    }
    void settel(QString tel){
        info_usertel=tel;
    }



private:
    Ui::changePassword *ui;
    QString userpassword;
    QTcpSocket *tcpSocket;
    QString info_username;
    QString info_userID;
    QString info_usertel;
    QString info_usergender;
    QString info_useremail;

private slots:
    void on_exit_clicked();
    void confirm_clicked();

};

#endif // CHANGEPASSWORD_H
