#-------------------------------------------------
#
# Project created by QtCreator 2016-12-15T15:49:21
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Operator
TEMPLATE = app

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = S.K.K KST-415
QMAKE_TARGET_PRODUCT = Students Registration System [Operator]
QMAKE_TARGET_DESCRIPTION = Course project
QMAKE_TARGET_COPYRIGHT = S.K.K KST-415

SOURCES += main.cpp\
        mainmenu.cpp \
    loginwindow.cpp \
    dbinterface.cpp

HEADERS  += mainmenu.h \
    loginwindow.h \
    dbinterface.h

FORMS    += mainmenu.ui \
    loginwindow.ui

RESOURCES += \
    res.qrc

RC_ICONS = Icon.ico
