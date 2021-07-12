#ifndef COMMENT_H
#define COMMENT_H

#include <QDialog>

namespace Ui {
class comment;
}

class comment : public QDialog
{
    Q_OBJECT

public:
    explicit comment(QWidget *parent = nullptr);
    ~comment();

private:
    Ui::comment *ui;


private slots:
    void on_send_clicked();

};

#endif // COMMENT_H
