#include "WelcomeDialog.h"
#include "ui_WelcomeDialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);
    connect(ui->goonButton, SIGNAL(clicked(bool)), this, SLOT(goon()));
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::goon(){     //继续显示软件的主界面
    this->hide();         //欢迎界面隐藏
    mainwidget.show();         //展示主界面
//    landwidget.show();     //展示登录界面
}
