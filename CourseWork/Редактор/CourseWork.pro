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
