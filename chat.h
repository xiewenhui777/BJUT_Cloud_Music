#ifndef CHAT_H
#define CHAT_H

#include <QDialog>

namespace Ui {
class chat;
}

class chat : public QDialog
{
    Q_OBJECT

public:
    explicit chat(QWidget *parent = nullptr);
    ~chat();
    void setName(QString name){
        friname=name;
    }
    void showInfo();

private:
    Ui::chat *ui;
    QString friname;
};

#endif // CHAT_H
