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
    controlpreprocesamiento.cpp \
    controlsegmentacion.cpp \
    dialogocaracteristicas.cpp \
    controlobtencioncaracteristicas.cpp \
    controlficheros.cpp \
    controlredneuronal.cpp

HEADERS  += mainwindow.h \
    controlpreprocesamiento.h \
    controlsegmentacion.h \
    dialogocaracteristicas.h \
    controlobtencioncaracteristicas.h \
    controlficheros.h \
    controlredneuronal.h

FORMS    += mainwindow.ui \
    dialogocaracteristicas.ui


INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /home/nicolas/Boost

LIBS += `pkg-config opencv --libs`
LIBS += -L/usr/lib/ -lboost_filesystem -lboost_system

DISTFILES += \
    ../../../Downloads/search102.png \
    ../../../../Downloads/1432827147_package_graphics.png

RESOURCES += \
    resources.qrc
