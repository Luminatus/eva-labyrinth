#-------------------------------------------------
#
# Project created by QtCreator 2019-04-29T06:20:17
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = test
CONFIG   += console
CONFIG   += testcase
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH = ../app


SOURCES += \
    Sources/gamesession_test.cpp \
    ../app/Sources/game_map.cpp \
    ../app/Sources/gamesession_model.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    Headers/gamesession_test.h \
    ../app/Headers/datastructures.h \
    ../app/Headers/game_map.h \
    ../app/Headers/gamesession_model.h
