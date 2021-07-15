#ifndef INFORMATION_H
#define INFORMATION_H

#include <QDialog>
#include "chuanshu.h"
#include <QTcpSocket>
namespace Ui {
class information;
}

class information : public QDialog
{
    Q_OBJECT
private:
    Ui::information *ui;
    QString info_username;
        QString info_userID;
        QString info_usertel;
        QString info_usergender;
        QString info_useremail;
        QTcpSocket *tcpSocket;
public:
    explicit information(QWidget *parent = nullptr);
    ~information();
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
       void info_show();
       void setSocket(QTcpSocket *soc)
       {
           tcpSocket = soc;
       }
private slots:
    void edit_clicked();
    void save_clicked();
};

#endif // INFORMATION_H
