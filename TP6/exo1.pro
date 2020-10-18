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

SOURCES += \
	exo1.cpp \
	lib/mainwindow.cpp \
	lib/array.cpp \
	lib/thread.cpp \
	lib/graph.cpp \
	lib/tp6.cpp

HEADERS += \
	lib/mainwindow.h \
	lib/array.h \
	lib/thread.h \
	lib/graph.h \
	lib/tp6.h
