#ifndef USERINFO_H
#define USERINFO_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>


extern int quit_login;       //储存是否退出的状态
namespace Ui {
class userInfo;
}

class userInfo : public QWidget
{
    Q_OBJECT

public:
    explicit userInfo(QWidget *parent = nullptr);
    ~userInfo();

private:
    Ui::userInfo *ui;
    QDialog *icoDia1;
    QVBoxLayout *layout_fri1;
    QList <QPushButton *> icolist1;
    QString icon1;

private slots:          //响应时间需要单独加一个slot!!!
    void on_exit_clicked();
    void on_headimage_clicked();
};

#endif // USERINFO_H
