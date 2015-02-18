#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T10:01:07
# For MPI version needs to write `qmake PartsEngine.pro "MODES=mpi"` in console.
#
#-------------------------------------------------

TARGET = PartsEngine
QT       += core

SUBDIRS += tests

QT       -= gui

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
    StateMachineFree.h \

contains(MODES,mpi) {
    TARGET = PartsEngineMpi
    message(Build with MPI)
    HEADERS += PartArrayMPI.h
    SOURCES += PartArrayMPI.cpp
    INCLUDEPATH += /usr/include/mpi
    DEPENDPATH += /usr/include/mpi

    QMAKE_CXX = mpicxx
    QMAKE_CXX_RELEASE = $$QMAKE_CXX
    QMAKE_CXX_DEBUG = $$QMAKE_CXX
    QMAKE_LINK = $$QMAKE_CXX
    QMAKE_CC = mpicc

} else {
    message(Build without an MPI)
}

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

