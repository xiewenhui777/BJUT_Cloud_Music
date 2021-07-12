#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QTcpSocket>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
//#include<qeventloop.h>
//#include<qevent.h>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();
    void setSocket(QTcpSocket *soc )
    {
        tcpSocket = soc;
    }
    void regiSucce(QString info);

    int state=0;
//    int doExec();
//    enum{Accepted, Rejected};

private slots:
    void on_pushButton_clicked();

    void on_headimage_clicked();
    void socket_Read_Data();
//    int result;
//    QEventLoop *loop;

private:
    Ui::Register *ui;
    QTcpSocket *tcpSocket;
    QDialog *icoDia;
    QVBoxLayout *layout_fri;
    QList <QPushButton *> icolist;
    QString icon;

protected:
   void closeEvent(QCloseEvent *event);


};

#endif // REGISTER_H
