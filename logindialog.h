#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include "register.h"
#include "forgetpassword.h"

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
private slots:
};

#endif // LOGINDIALOG_H
