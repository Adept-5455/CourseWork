#-------------------------------------------------
#
# Project created by QtCreator 2016-12-15T16:06:20
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Redactor
TEMPLATE = app

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = S.K.K KST-415
QMAKE_TARGET_PRODUCT = Students Registration System [Operator]
QMAKE_TARGET_DESCRIPTION = Course project
QMAKE_TARGET_COPYRIGHT = S.K.K KST-415

SOURCES += main.cpp\
        mainmenu.cpp \
    loginwindow.cpp \
    addoperatorwindow.cpp \
    dbinterface.cpp

HEADERS  += mainmenu.h \
    loginwindow.h \
    addoperatorwindow.h \
    dbinterface.h

FORMS    += mainmenu.ui \
    loginwindow.ui \
    addoperatorwindow.ui

RC_ICONS = Icon.ico

RESOURCES += \
    res.qrc
