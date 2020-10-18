#-------------------------------------------------
#
# Project created by QtCreator 2019-02-03T21:14:48
#
#-------------------------------------------------

lessThan(QT_MAJOR_VERSION, 4): error("requires Qt>4")

QT       += core gui widgets

TARGET = Algorithm_TD5_exo1
TEMPLATE = app
CONFIG += c++11

DEFINES += USE_NODE

SOURCES += \
		exo1.cpp \
        lib/mainwindow.cpp \
		lib/array.cpp \
    lib/binarytree.cpp \
    lib/heap.cpp \
    lib/thread.cpp \
    lib/tp5.cpp \
    lib/hashtable.cpp

HEADERS += \
        lib/mainwindow.h \
		lib/array.h \
    lib/binarytree.h \
    lib/heap.h \
    lib/thread.h \
    lib/tp5.h \
    lib/hashtable.h
