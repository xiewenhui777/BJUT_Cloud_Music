#-------------------------------------------------
#
# Project created by QtCreator 2019-12-11T10:44:34
#
#-------------------------------------------------

QT       += core gui sql
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BJUT_cloud_music
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    Music.cpp \
    MainWidget.cpp \
    MusicListWidget.cpp \
    MusicList.cpp \
    MusicListDialog.cpp \
    LyricWidget.cpp \
    MusicSearchDialog.cpp \
    WelcomeDialog.cpp \
    PersonalDialog.cpp \
    landwidget.cpp \
    register.cpp \
    changepassword.cpp \
    forgetpassword.cpp \
    information.cpp \
    chat.cpp \
    comment.cpp \
    feedbackdialog.cpp \
    frienddialog.cpp \
    frilist.cpp \
    chuanshu.cpp \
    logindialog.cpp

HEADERS += \
    Music.h \
    myQSS.h \
    MainWidget.h \
    MusicListWidget.h \
    MusicList.h \
    MusicListDialog.h \
    LyricWidget.h \
    MusicSearchDialog.h \
    WelcomeDialog.h \
    PersonalDialog.h \
    landwidget.h \
    register.h \
    changepassword.h \
    forgetpassword.h \
    information.h \
    chat.h \
    comment.h \
    feedbackdialog.h \
    frienddialog.h \
    frilist.h \
    chuanshu.h \
    logindialog.h

FORMS += \
    MusicListDialog.ui \
    mainWidget.ui \
    LyricWidget.ui \
    MusicSearchDialog.ui \
    WelcomeDialog.ui \
    PersonalDialog.ui \
    landwidget.ui \
    register.ui \
    changepassword.ui \
    forgetpassword.ui \
    information.ui \
    chat.ui \
    comment.ui \
    feedbackdialog.ui \
    frienddialog.ui \
    logindialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    imagefile.qrc


RC_ICONS = BJUT_cloud_music.ico
