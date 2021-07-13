#ifndef FRIENDDIALOG_H
#define FRIENDDIALOG_H

#include <QDialog>
#include "MusicList.h"
#include "MusicListWidget.h"
#include <QMenu>

namespace Ui {
class FriendDialog;
}

class FriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FriendDialog(QWidget *parent = nullptr);
    ~FriendDialog();

private:
    Ui::FriendDialog *ui;

private slots:
    void addFri();
};

#endif // FRIENDDIALOG_H
