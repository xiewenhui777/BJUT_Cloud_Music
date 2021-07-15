#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include "register.h"
#include "forgetpassword.h"
#include "chuanshu.h"

namespace Ui {
    class testDialog;
}

class testDialog : public QDialog
{
    Q_OBJECT

public:
    explicit testDialog(QWidget *parent = nullptr);
    ~testDialog();

    void setSocket(QTcpSocket *soc)
    {
        tcpSocket = soc;

    }
    QString loginID;
private:
    Ui::testDialog *ui;
    QTcpSocket *tcpSocket;
    Register *regi;
    forgetPassword forget1;
    QString str;

private slots:
    void on_login_clicked();
    void on_registeButton_clicked();
    void findPasswod_clicked();
//    void socket_Read_Data();
};

#endif // TESTDIALOG_H
