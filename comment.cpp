#include "comment.h"
#include "ui_comment.h"
#include <QDebug>

comment::comment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::comment)
{
    ui->setupUi(this);
    ui->messBrowser->setEnabled(false);
    ui->lineEdit->setEnabled(false);            //不可编辑状态
    connect(ui->sendButton,SIGNAL(clicked()), this,SLOT(on_send_clicked()));
}

comment::~comment()
{
    delete ui;
}



void comment::on_send_clicked(){        //发送文本框中的内容
    QString content=ui->messEdit->toPlainText();      //获取文本框总的内容
    qDebug()<<content;
    ui->messEdit->clear();
}
