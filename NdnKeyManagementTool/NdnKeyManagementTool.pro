#-------------------------------------------------
#
# Project created by QtCreator 2013-04-12T12:34:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NdnKeyManagementTool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    context.cpp \
    application.cpp \
    daemon.cpp \
    organizersession.cpp \
    participantsession.cpp \
    sharedkey.cpp \
    peer.cpp

HEADERS  += mainwindow.h \
    context.h \
    application.h \
    daemon.h \
    organizersession.h \
    participantsession.h \
    sharedkey.h \
    peer.h

FORMS    += mainwindow.ui
