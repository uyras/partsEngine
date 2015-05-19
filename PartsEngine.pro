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
    distributionLaw.cpp \
    config.cpp \
    StateMachine.cpp \
    StateMachineFree.cpp \
    statemachinebase.cpp

HEADERS += Vect.h \
    PartArray.h \
    Part.h \
    distributionLaw.h \
    config.h \
    StateMachine.h \
    StateMachineFree.h \
    statemachinebase.h


contains(MODES,boost) {
    CONFIG(release,debug|release){
        TARGET = PartsEngineBoost
    }

    message(Build with Boost.mpi library);
    QMAKE_CXX = mpicxx
    QMAKE_CXX_RELEASE = $$QMAKE_CXX
    QMAKE_CXX_DEBUG = $$QMAKE_CXX
    QMAKE_CXXFLAGS_DEBUG = -O1 -fno-omit-frame-pointer -g
    QMAKE_LINK = $$QMAKE_CXX
    QMAKE_CC = mpicc

    HEADERS += partarrayboost.h
    HEADERS += PartArrayMPI.h
    SOURCES += PartArrayMPI.cpp\
        statemachinegmp.cpp
    LIBS+= -lboost_mpi -lboost_serialization
    DEFINES += WITH_BOOST=true
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

