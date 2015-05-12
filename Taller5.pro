#-------------------------------------------------
#
# Project created by QtCreator 2015-04-29T01:31:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Taller5
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    controlpreprocesamiento.cpp

HEADERS  += mainwindow.h \
    controlpreprocesamiento.h

FORMS    += mainwindow.ui


INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2

LIBS += `pkg-config opencv --libs`

DISTFILES += \
    ../../../Downloads/search102.png
