#ifndef COMMENT_H
#define COMMENT_H
#include <QTcpSocket>
#include <QDialog>
#include <QString>
#include <QUrl>
namespace Ui {
class comment;
}

class comment : public QDialog
{
    Q_OBJECT
private:
    Ui::comment *ui;

public:
    QString comid;
    QString useid;
    QString userid;
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
    void setcomid(QString id){
        this->comid=id;
    }
    void setuser(QString id){
        this->useid=id;
    }
private slots:
    void on_send_clicked();

    void on_fresh_clicked();
    void on_delete_2_clicked();

};

#endif // COMMENT_H
