#-------------------------------------------------
#
# Project created by QtCreator 2019-03-22T11:49:59
#
#-------------------------------------------------

QT       += core gui

TARGET = lorenz
TEMPLATE = lib
CONFIG += plugin

CONFIG(debug, debug|release) {
    BUILDTYPE = debug
} else {
    BUILDTYPE = release }

DESTDIR = ../../DynModels/$${BUILDTYPE}/plugins

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        lorenzmodel.cpp

HEADERS += \
        lorenzmodel.h
DISTFILES += lorenz.json 

unix {
    target.path = /usr/lib
    INSTALLS += target
}
