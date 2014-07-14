#-------------------------------------------------
#
# Project created by QtCreator 2014-07-15T10:13:48
#
#-------------------------------------------------

QT       += core testlib

QT       -= gui

TARGET = tests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    partarraytest2D.cpp

HEADERS += \
    partarraytest2D.h

INCLUDEPATH += ../
LIBS += -L$$PWD/../ -lPartsEngine
