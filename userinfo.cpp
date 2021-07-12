#include "userinfo.h"
#include "ui_userinfo.h"
#include <QDebug>
#include <QDialog>

userInfo::userInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userInfo)
{
    ui->setupUi(this);

    icon1 = "00";
    icoDia1 = new QDialog(this);
    layout_fri1 = new QVBoxLayout(icoDia1);
    layout_fri1->setMargin(20);
    layout_fri1->setAlignment(Qt::AlignBaseline);
    for(int i=0;i<7;i++)
    {
        QString path = QString(":/image/image/headicon/0%1.ico").arg(i);            //注意文件夹的路径名称（与diamante中的资源保持一致）
//        QPushButton *bu_temp = new QPushButton();
        QPushButton *bu_temp = new QPushButton(icoDia1);          //可能是 因为dialog中不能嵌套dialog才使得出bug
        bu_temp->setIcon(QPixmap(path));
        qDebug()<<"path is "<<path;
        icolist1.append(bu_temp);
        layout_fri1->addWidget(bu_temp);

        connect(bu_temp,&QPushButton::clicked,
                [=]() mutable
                {
                    this->icoDia1->hide();
                    ui->headimage->setIcon(QPixmap(path));
                    //ui->messEdit->append("\n");
                    icon1[0] = path[7];
                    icon1[1] = path[8];
                }
                );

    }

    connect(ui->exitButton,SIGNAL(clicked()), this,SLOT(on_exit_clicked()));
}

userInfo::~userInfo()
{
    delete ui;
}

void userInfo::on_exit_clicked(){ //隐藏起个人信息界面
    qDebug()<<"hhhhhh"<<endl;
    quit_login=1;   //代表退出当前登录
    close();
    this->hide();
//    exec();     //进程结束

//    qDebug()<<"hhhhhh"<<endl;
//    this->hide();
}

void userInfo::on_headimage_clicked()
{
    icoDia1->show();
}
