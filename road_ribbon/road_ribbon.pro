#-------------------------------------------------
#
# Project created by QtCreator 2014-01-09T21:37:08
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = road_ribbon
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lsfml-graphics -lGL -lGLU -lsfml-window -lsfml-system

SOURCES += main.cpp \
    gpsdataparser.cpp \
    vector2d.cpp \
    plainroadbuilder.cpp

HEADERS += \
    gpsdataparser.h \
    vector2d.h \
    plainroadbuilder.h
