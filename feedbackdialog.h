#ifndef FEEDBACKDIALOG_H
#define FEEDBACKDIALOG_H

#include <QDialog>

namespace Ui {
class feedbackDialog;
}

class feedbackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit feedbackDialog(QWidget *parent = nullptr);
    ~feedbackDialog();

private:
    Ui::feedbackDialog *ui;

private slots:
    void on_send_clicked();
};

#endif // FEEDBACKDIALOG_H
