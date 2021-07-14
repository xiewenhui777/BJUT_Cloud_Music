#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include "register.h"
#include "forgetpassword.h"
//#include "chuanshu.h

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = nullptr);
    ~loginDialog();

private:
    Ui::loginDialog *ui;
    QTcpSocket *tcpSocket;
//    Register *regi;
//    forgetPassword forget1;
//    QString str;

private slots:
//    void on_login_clicked();
//    void on_registeButton_clicked();
////    void findPasswod_clicked();
};

#endif // LOGINDIALOG_H
