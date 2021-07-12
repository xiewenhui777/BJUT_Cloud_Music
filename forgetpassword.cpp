#include "forgetpassword.h"
#include "ui_forgetpassword.h"

forgetPassword::forgetPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::forgetPassword)
{
    ui->setupUi(this);
    connect(ui->cancel,SIGNAL(clicked()), this,SLOT(on_exit_clicked()));
    connect(ui->confirm,SIGNAL(clicked()), this,SLOT(confirm_clicked()));
}

forgetPassword::~forgetPassword()
{
    delete ui;
}

void forgetPassword::on_exit_clicked(){ //不做任何操作  直接出退出当前界面
    close();
    exec();     //进程结束
}

void forgetPassword::confirm_clicked(){     //确认找回密码
    //找回密码
    close();    //完成后 退出当前界面
    exec();     //进程结束
}
