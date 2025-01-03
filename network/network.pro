#-------------------------------------------------
#
# Project created by QtCreator 2024-12-24T14:07:40
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TEMPLATE = app
QT       += core gui
QT       += network
QT       += xml

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
        facedk.cpp \
        filecntdlg.cpp \
        filesrvdlg.cpp \
        login_reg.cpp \
        logindialog.cpp \
        main.cpp \
        mainwindow.cpp \
        menu.cpp \
        splashscreen.cpp \


HEADERS += \
        facedk.h \
        filecntdlg.h \
        filesrvdlg.h \
        login_reg.h \
        logindialog.h \
        mainwindow.h \
        menu.h \
        splashscreen.h \


FORMS += \
        facedk.ui \
        filecntdlg.ui \
        filesrvdlg.ui \
        login_reg.ui \
        logindialog.ui \
        mainwindow.ui \
        menu.ui \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    customer.qrc \
    dialog.qrc \
    service.qrc

DISTFILES += \
    userlog.xml
