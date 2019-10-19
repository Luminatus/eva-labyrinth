#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T17:18:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = app
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../test

SOURCES += main.cpp\
    Sources/mainwindow_view.cpp \
    Sources/mainwindow_model.cpp \
    Sources/gamesession_model.cpp \
    Sources/gamesession_view.cpp \
    Sources/tableview.cpp \
    Sources/viewdata.cpp \
    Sources/game_map.cpp

HEADERS  += \
    Headers/datastructures.h \
    Headers/mainwindow_view.h \
    Headers/mainwindow_model.h \
    Headers/gamesession_model.h \
    Headers/gamesession_view.h \
    Headers/tableview.h \
    Headers/viewdata.h \
    Headers/game_map.h

FORMS    += UI/mainwindow.ui \
    UI/gamesession.ui

RESOURCES +=

copydata.commands = $(COPY_DIR) $$PWD/Maps $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
