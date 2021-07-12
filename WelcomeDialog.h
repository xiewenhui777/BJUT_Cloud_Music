#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>
#include "mainWidget.h"
#include "landwidget.h"

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    ~WelcomeDialog();

private:
    Ui::WelcomeDialog *ui;
    LandWidget landwidget;      //登录界面的实例
//    MainWidget mainwidget;

private slots:
    void goon();
};

#endif // WELCOMEDIALOG_H
