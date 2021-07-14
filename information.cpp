#include "information.h"
#include "ui_information.h"

information::information(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::information)
{
    ui->setupUi(this);
    ui->email->setEnabled(false);
    ui->userID->setEnabled(false);
    ui->phone->setEnabled(false);
    ui->gender->setEnabled(false);
    ui->name->setEnabled(false);

    connect(ui->edit,SIGNAL(clicked()), this,SLOT(edit_clicked()));
    connect(ui->save,SIGNAL(clicked()), this,SLOT(save_clicked()));
}

information::~information()
{
    delete ui;
}


void information::edit_clicked(){
    ui->email->setEnabled(true);
    ui->userID->setEnabled(true);
    ui->phone->setEnabled(true);
    ui->gender->setEnabled(true);
    ui->name->setEnabled(true);
//    close();
//    exec();     //进程结束
}
void information::save_clicked(){
    close();
    exec();     //进程结束
}
void information::info_show(){      //设置个人信息显示
    ui->userID->setText(info_username);
    ui->name->setText(info_userID);
    ui->gender->setText(info_usertel);
    ui->phone->setText(info_usergender);
    ui->email->setText(info_useremail);
}
