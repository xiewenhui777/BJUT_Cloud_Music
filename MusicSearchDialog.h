#ifndef MUSICSEARCHDIALOG_H
#define MUSICSEARCHDIALOG_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class MusicSearchDialog;
}

class MusicSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MusicSearchDialog(QWidget *parent = nullptr);
    ~MusicSearchDialog();

    //传输函数  用于传输用户ID和socket
    void transfer(QTcpSocket *Socket,QString userid){
        tcpSocket=Socket;
        userID=userid;
    }

private:
    Ui::MusicSearchDialog *ui;
    QTcpSocket *tcpSocket;
    QString userID;         //用户ID

private slots:
    void search();
    void socket_Read_Data();
    void on_download_clicked();
};

#endif // MUSICSEARCHDIALOG_H
