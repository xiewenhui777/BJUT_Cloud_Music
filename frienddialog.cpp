#include "frienddialog.h"
#include "ui_frienddialog.h"
#include "frilist.h"

FriendDialog::FriendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FriendDialog)
{
    ui->setupUi(this);
    connect(ui->addfriend,SIGNAL(clicked()), this,SLOT(addFri()));
}

FriendDialog::~FriendDialog()
{
    delete ui;
}


void FriendDialog::addFri(){
    FriList *fr = new FriList(this);
    fr->addFriend(10001,"kid","00","hahah");
    fr->addFriend(10002,"赵满刚","03","呵呵呵呵");
}
