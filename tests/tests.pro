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


SOURCES += main.cpp

HEADERS += \
    VectTest2D.h \
    StateMachineTest.h \
    statemachinefreetest.h \
    squarespinicetest.h \
    randomtest.h \
    honeycombspinicetest.h \
    PartArrayTest.h \
    parttest.h

INCLUDEPATH += ../
LIBS += -L$$PWD/../ -lPartsEngine
