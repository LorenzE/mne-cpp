#-------------------------------------------------
#
# Project created by QtCreator 2016-11-23T14:51:10
#
#-------------------------------------------------

include (../../../../mne-cpp.pri)

CONFIG += console

QT       += core widgets

QT       -= gui

TARGET = myoarmband
TEMPLATE = lib

DEFINES += MYOARMBAND_LIBRARY

SOURCES += myoarmband.cpp \
            FormFiles/myoarmbandgui.cpp \
            myorealtimeproducer.cpp \
    myoarmbanddriver.cpp

HEADERS += myoarmband.h\
            myoarmband_global.h \
            FormFiles/myoarmbandgui.h \
            myorealtimeproducer.h \
    myoarmbanddriver.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$${MNE_LIBRARY_DIR}
CONFIG(debug, debug|release) {
    LIBS += -lMNE$${MNE_LIB_VERSION}Genericsd \
            -lMNE$${MNE_LIB_VERSION}Utilsd \
            -lMNE$${MNE_LIB_VERSION}Fiffd \
            -lscMeasd \
            -lscDispd \
            -lscSharedd
}
else {
    LIBS += -lMNE$${MNE_LIB_VERSION}Generics \
            -lMNE$${MNE_LIB_VERSION}Utils \
            -lMNE$${MNE_LIB_VERSION}Fiff \
            -lscMeas \
            -lscDisp \
            -lscShared
}

DESTDIR = $${MNE_BINARY_DIR}/mne_scan_plugins

INCLUDEPATH += $${EIGEN_INCLUDE_DIR}
INCLUDEPATH += $${MNE_INCLUDE_DIR}
INCLUDEPATH += $${MNE_SCAN_INCLUDE_DIR}

# Put generated form headers into the origin --> cause other src is pointing at them
UI_DIR = $${PWD}

unix: QMAKE_CXXFLAGS += -isystem $$EIGEN_INCLUDE_DIR

# suppress visibility warnings
unix: QMAKE_CXXFLAGS += -Wno-attributes

DISTFILES += \
    myoarmband.json

FORMS += \
    FormFiles/myoarmbandgui.ui

LIBS += -L$$PWD/lib/ -lmyo32
LIBS += -L$$PWD/lib/ -lmyo64

