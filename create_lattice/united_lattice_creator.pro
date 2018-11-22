QT += core
QT -= gui

CONFIG += c++11

TARGET = pyrochloreCsv
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

LIBS += -L../ -lPartsEngine

INCLUDEPATH += ../
DEPENDPATH += ../

PRE_TARGETDEPS += ../libPartsEngine.a

SOURCES += main.cpp \
    ../cagomelattice.cpp \
    ../hc_flower.cpp \
    ../triangularlattice.cpp \
    ../pyrochlorelattice.cpp

HEADERS += \
    ../cagomelattice.h \
    ../hc_flower.h \
    ../triangularlattice.h\
    ../pyrochlorelattice.h
