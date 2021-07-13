#ifndef LANDWIDGET_H
#define LANDWIDGET_H

#include <QWidget>
#include <QTcpSocket>
//#include "register.h"
#include "mainWidget.h"
//#include "forgetpassword.h"
#include "chuanshu.h"

namespace Ui {
class LandWidget;
}

class LandWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LandWidget(QWidget *parent = nullptr);
    ~LandWidget();




private slots:
    void on_login_clicked();



private:
    Ui::LandWidget *ui;
    QTcpSocket *tcpSocket;

    MainWidget *mainwidget;      //主界面的实例

    QString str;

};

#endif // LANDWIDGET_H
