#-------------------------------------------------
#
# Project created by QtCreator 2013-07-19T22:28:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app-selector
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        item.cpp \
        jsonparser.cpp

HEADERS  += mainwindow.h \
        item.h \
        sajson.h \
        jsonparser.h

FORMS    += mainwindow.ui
