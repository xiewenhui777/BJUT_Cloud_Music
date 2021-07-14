#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>

namespace Ui {
class changePassword;
}

class changePassword : public QDialog
{
    Q_OBJECT

public:
    explicit changePassword(QWidget *parent = nullptr);
    ~changePassword();
    void setpassword(QString password){
        userpassword=password;
    }

private:
    Ui::changePassword *ui;
    QString userpassword;

private slots:
    void on_exit_clicked();
    void confirm_clicked();

};

#endif // CHANGEPASSWORD_H
