QT       += core gui
QT       += core gui qml quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adduser.cpp \
    findpage.cpp \
    imagethread.cpp \
    loginpage.cpp \
    main.cpp \
    mainwindow.cpp \
    mymusic.cpp \
    mythread.cpp \
    playlist.cpp \
    playpage.cpp
 #    qmlcontrol.cpp

HEADERS += \
    adduser.h \
    findpage.h \
    imagethread.h \
    loginpage.h \
    mainwindow.h \
    mymusic.h \
    mythread.h \
    playlist.h \
    playpage.h
 #    qmlcontrol.h

FORMS += \
    adduser.ui \
    findpage.ui \
    loginpage.ui \
    mainwindow.ui \
    mymusic.ui \
    playlist.ui \
    playpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    logo.jpg \
    test.jpg

RESOURCES += \
    Resource/Resource.qrc

QT += multimedia
QT += quick
QT += network
