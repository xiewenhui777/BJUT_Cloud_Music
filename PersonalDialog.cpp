#include "PersonalDialog.h"
#include "ui_PersonalDialog.h"
#include <QDebug>
//#include <QDialog>

int quit_login;     //全局变量
PersonalDialog::PersonalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonalDialog)
{
    ui->setupUi(this);

    icon1 = "00";
    icoDia1 = new QDialog(this);
    layout_fri1 = new QVBoxLayout(icoDia1);
    layout_fri1->setMargin(20);
    layout_fri1->setAlignment(Qt::AlignBaseline);
    //添加头像
    for(int i=0;i<7;i++)
    {
        QString path = QString(":/image/image/headicon/0%1.jpg").arg(i);            //注意文件夹的路径名称（与diamante中的资源保持一致）
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
    connect(ui->changePasswod,SIGNAL(clicked()), this,SLOT(changePasswod_clicked()));
    connect(ui->info,SIGNAL(clicked()), this,SLOT(info_clicked()));
    connect(ui->feedbackButton,SIGNAL(clicked()), this,SLOT(on_feedback_clicked()));
    connect(ui->fributton,SIGNAL(clicked()), this,SLOT(on_friend_clicked()));
}

PersonalDialog::~PersonalDialog()
{
    delete ui;
}

void PersonalDialog::on_exit_clicked(){ //隐藏起个人信息界面
    qDebug()<<"hhhhhh"<<endl;
    quit_login=1;   //代表退出当前登录
    close();
    exec();     //进程结束
}

void PersonalDialog::on_headimage_clicked()
{
    icoDia1->show();
}

void PersonalDialog::changePasswod_clicked()        //修改密码的按钮
{
    change1.show();
    change1.exec();
}

void PersonalDialog::info_clicked()        //显示个人资料
{
    info1.setuserID(userID);
    info1.setname(username);
    info1.setemail(useremail);
    info1.setgender(usergender);
    info1.settel(usertel);
    info1.info_show();      //个人信息先显示
    info1.show();
    info1.exec();
}

void PersonalDialog::on_feedback_clicked(){
    feedback.show();
    feedback.exec();
}

void PersonalDialog::on_friend_clicked(){
    fri.show();
    fri.exec();
}

void PersonalDialog::showInfo(){        //显示个人信息（如用户名
    ui->namelabel->setText(username);       //设置用户名
}
