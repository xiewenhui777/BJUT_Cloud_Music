#include "chat.h"
#include "ui_chat.h"

chat::chat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chat)
{
    ui->setupUi(this);
}

chat::~chat()
{
    delete ui;
}
void chat::showInfo(){        //显示好友名称
    ui->friendname->setText(friname);
}
