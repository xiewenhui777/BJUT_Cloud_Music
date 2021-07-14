#ifndef COMMENT_H
#define COMMENT_H
#include <QTcpSocket>
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
    QString musicname;
    void setMusicname(QString name){
        musicname=name;
        qDebug()<<musicname;
    }
    QTcpSocket *tcpSocket;
    void settcp(QTcpSocket *tcp){
        tcpSocket =tcp;
    }
    void showcomment();
    void setrecv(QString rec);
private:
    Ui::comment *ui;


private slots:
    void on_send_clicked();

    void on_fresh_clicked();
};

#endif // COMMENT_H
