#ifndef FORGETPASSWORD_H
#define FORGETPASSWORD_H

#include <QDialog>

namespace Ui {
class forgetPassword;
}

class forgetPassword : public QDialog
{
    Q_OBJECT

public:
    explicit forgetPassword(QWidget *parent = nullptr);
    ~forgetPassword();

private:
    Ui::forgetPassword *ui;

private slots:
    void on_exit_clicked();
    void confirm_clicked();
};

#endif // FORGETPASSWORD_H
