#-------------------------------------------------
#
# Project created by QtCreator 2019-07-24T19:48:44
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChessBy
TEMPLATE = app


SOURCES += main.cpp\
        board.cpp \
    stone.cpp

HEADERS  += board.h \
    stone.h

CONFIG +=c++11
