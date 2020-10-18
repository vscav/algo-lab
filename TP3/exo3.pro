#-------------------------------------------------
#
# Project created by QtCreator 2019-02-03T21:14:48
#
#-------------------------------------------------

lessThan(QT_MAJOR_VERSION, 4): error("requires Qt>4")

QT       += core gui widgets

TARGET = Algorithm_TD3_exo3
TEMPLATE = app
CONFIG += c++11

SOURCES += \
		exo3.cpp \
		lib/mainwindow.cpp \
		lib/thread.cpp \
		lib/array.cpp \
		lib/binarytree.cpp \
		lib/tp3.cpp \

HEADERS += \
		lib/mainwindow.h \
		lib/thread.h \
		lib/array.h \
		lib/binarytree.h \
		lib/tp3.h \
