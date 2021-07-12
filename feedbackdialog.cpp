#include "feedbackdialog.h"
#include "ui_feedbackdialog.h"
#include<QDebug>

feedbackDialog::feedbackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::feedbackDialog)
{
    ui->setupUi(this);
    ui->messBrowser->setEnabled(false);
    connect(ui->sendButton,SIGNAL(clicked()), this,SLOT(on_send_clicked()));
}

feedbackDialog::~feedbackDialog()
{
    delete ui;
}


void feedbackDialog::on_send_clicked(){
    QString content=ui->messEdit->toPlainText();      //获取文本框总的内容
    qDebug()<<content;
    ui->messEdit->clear();
}
