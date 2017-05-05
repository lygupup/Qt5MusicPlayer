#-------------------------------------------------
#
# Project created by QtCreator 2017-04-24T15:25:14
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IcePlayer
TEMPLATE = app


SOURCES += main.cpp\
        iceplayer.cpp \
    aboutform.cpp \
    icelabel.cpp \
    lrclabel.cpp \
    icelrc.cpp

HEADERS  += iceplayer.h \
    aboutform.h \
    icelabel.h \
    lrclabel.h \
    icelrc.h

FORMS += \
    IcePlayer.ui

RESOURCES += \
    resource.qrc
