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

QTPLUGIN += QSQLMYSQL

SOURCES += main.cpp \
    loginwindow.cpp \
    mainmenu.cpp \
    addoperatorwindow.cpp

HEADERS  += \
    loginwindow.h \
    mainmenu.h \
    addoperatorwindow.h

FORMS    += \
    loginwindow.ui \
    mainmenu.ui \
    addoperatorwindow.ui
