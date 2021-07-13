#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

namespace Ui {
class registerDialog;
}

class registerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit registerDialog(QWidget *parent = nullptr);
    ~registerDialog();
    void setSocket(QTcpSocket *soc )
    {
        tcpSocket = soc;
    }
    void regiSucce(QString info);

    int state=0;

private:
    Ui::registerDialog *ui;
    QTcpSocket *tcpSocket;
    QDialog *icoDia;
    QVBoxLayout *layout_fri;
    QList <QPushButton *> icolist;
    QString icon;


private slots:
    void on_pushButton_clicked();

    void on_headimage_clicked();
    void socket_Read_Data();


protected:
   void closeEvent(QCloseEvent *event);
};

#endif // REGISTERDIALOG_H
