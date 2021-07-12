#include "MainWidget.h"
#include "WelcomeDialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WelcomeDialog welcome;      //在main函数中先展示欢迎界面 MainWidget主界面在欢迎界面之后展示
    welcome.show();

    //MainWidget w;
    //w.show();
    
    return a.exec();
}
