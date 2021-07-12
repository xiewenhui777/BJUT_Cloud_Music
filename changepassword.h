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

private:
    Ui::changePassword *ui;

private slots:
    void on_exit_clicked();
    void confirm_clicked();

};

#endif // CHANGEPASSWORD_H
