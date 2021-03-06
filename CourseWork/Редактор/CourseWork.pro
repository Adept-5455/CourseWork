#-------------------------------------------------
#
# Project created by QtCreator 2016-12-04T12:48:54
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CourseWork
TEMPLATE = app

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = S.K.K KST-415
QMAKE_TARGET_PRODUCT = Students Registration System [Operator]
QMAKE_TARGET_DESCRIPTION = Course project
QMAKE_TARGET_COPYRIGHT = S.K.K KST-415

QTPLUGIN += QSQLMYSQL

SOURCES += main.cpp \
    mainmenu.cpp \
    addoperatorwindow.cpp \
    loginwindow.cpp

HEADERS  += \
    mainmenu.h \
    addoperatorwindow.h \
    loginwindow.h

FORMS    += \
    mainmenu.ui \
    addoperatorwindow.ui \
    loginwindow.ui

RESOURCES += \
    res.qrc

RC_ICONS = Icon.ico
