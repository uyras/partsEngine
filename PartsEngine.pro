#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T10:01:07
#
#-------------------------------------------------

TARGET = PartsEngine
CONFIG -= qt

SUBDIRS += tests

SOURCES += Vect.cpp \
    PartArray.cpp \
    Part.cpp \
    INIReader.cpp \
    ini.c \
    distributionLaw.cpp \
    config.cpp \
    StateMachine.cpp \
    StateMachineFree.cpp

HEADERS += Vect.h \
    typizator.h \
    PartArray.h \
    Part.h \
    INIReader.h \
    ini.h \
    distributionLaw.h \
    config.h \
    StateMachine.h \
    StateMachineFree.h

CONFIG(debug,debug|release) {
    SOURCES += main.cpp
    TEMPLATE = app
    CONFIG += console
}

CONFIG(release,debug|release){
    TEMPLATE = lib
    CONFIG += staticlib
    DESTDIR = $$PWD
}

OTHER_FILES += \
    README.md \
    .gitignore \
    makefile

