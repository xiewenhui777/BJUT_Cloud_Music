#ifndef CHUANSHU_H
#define CHUANSHU_H

#include <QObject>
#include <QDebug>
#include <hash_map>

class chuanshu : public QObject
{
    Q_OBJECT
public:
    explicit chuanshu(QObject *parent = nullptr);

public:

       int type; // 1私聊 0上下线更新 -1下线请求 2请求发送文件 3->确定接收文件

        QString info;//send
        QString timer;//send
        QString name;//id
        QString fileName;
        QString wantsendto;

        int size;
        QString ip;
        int port;
        int getSize() {
                return size;
            }

         void setSize(int size) {
                this->size = size;
            }

         QString getFileName() {
                return fileName;
            }

          void setFileName(QString fileName) {
                this->fileName = fileName;
            }

           int getType() {
                return type;
            }

            void setType(int type) {
                this->type = type;
            }


            QString getInfo() {
                return info;
            }

            void setInfo(QString info) {
                this->info = info;
            }

            QString getTimer() {
                return timer;
            }

            void setTimer(QString timer) {
                this->timer = timer;
            }

            QString getName() {
                return name;
            }

            void setName(QString name) {
                this->name = name;
            }

            QString getIp() {
                return ip;
            }

            void setIp(QString ip) {
                this->ip = ip;
            }

           int getPort() {
                return port;
            }

            void setPort(int port) {
                this->port = port;
            }

            QString getWantsendto()
            {
                return wantsendto;
            }

            void setWantsendto(QString wantsendto)
            {
                this->wantsendto = wantsendto;
            }


            QString toString()
                {
                    QString s1="";
                    QString s2="";
                    QString sb = s1 + s2 +
                            (QString)type +
                            "#" +
                            info + "" +
                            "#" +
                            timer + "" +
                            "#" +
                            name + "" +
                            "#" +
                            fileName + "" +
                            "#" +
                            wantsendto + "" +
                            "#" +
                            (QString)size +
                            "#" +
                            ip + "" +
                            "";//最后必须加#
                    return sb;
                }

//            chuanshu()
//                 {              //构造函数
//                     this->type = 0;
//                     this->info = "";
//                     this->timer = "";
//                     this->name = "";
//                     this->fileName = "";
//                     this->wantsendto = "";
//                     this->size = 0;
//                     this->ip = "";
//                 }

                        chuanshu(QString dollerstr)
                            {
                                QStringList spstr=dollerstr.split("#");
                                this->type = spstr[0].toInt();;
                                this->info = spstr[1];
                                this->timer = spstr[2];
                                this->name = spstr[3];
                                this->fileName = spstr[4];
                                this->wantsendto = spstr[5];
                                this->size = spstr[6].toInt();
                                this->ip = spstr[7];
                            }


signals:

public slots:
};

#endif // CHUANSHU_H












