#-------------------------------------------------
#
# Project created by QtCreator 2014-04-03T16:21:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GUI_Quadcopter
TEMPLATE = app


SOURCES += main.cpp\
        qcustomplot.cpp \
        gui_interface.cpp \
        pollthread.cpp \
        ../Monitor/monitor.cpp

HEADERS  += gui_interface.h \
        qcustomplot.h \
        pollthread.h \
        ../../include/Monitor/monitor.hpp

FORMS    += gui_interface.ui
