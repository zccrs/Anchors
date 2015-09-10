#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T14:35:41
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Anchors
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    anchors.cpp \
    dragwidget.cpp

HEADERS  += mainwindow.h \
    anchors.h \
    dragwidget.h

FORMS    += mainwindow.ui
