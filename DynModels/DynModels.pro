#-------------------------------------------------
#
# Project created by QtCreator 2018-01-09T11:50:32
#
#-------------------------------------------------

QT       += core gui charts datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = DynModels
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Tell the qcustomplot header that it will be used as library:
DEFINES += QCUSTOMPLOT_USE_LIBRARY
#DEFINES += QCUSTOMPLOT_USE_OPENGL

# Link with debug version of qcustomplot if compiling in debug mode, else with release library:
CONFIG(debug, release|debug) {
  win32:QCPLIB = qcustomplotd2
  else: QCPLIB = qcustomplotd
} else {
  win32:QCPLIB = qcustomplot2
  else: QCPLIB = qcustomplot
}

LIBS += -L./ -l$$QCPLIB

SOURCES += \
        main.cpp \
    pluginhandler.cpp \
    diffworker.cpp \
    mainwindow.cpp \
    intergrationsettingframe.cpp \
    coefswidget.cpp \
    coefitem.cpp \
    populationsetframe.cpp \
    resultwidget.cpp \
    plot2d.cpp \
    phaseportrait.cpp \
    commonplot.cpp \
    tabplot3d.cpp \
    plot3d.cpp \
    tabtable.cpp \
    tabledatamodel.cpp \
    external/exportdialog.cpp \
    external/dataloader.cpp \
    dialogs/resultdialog.cpp \
    dialogs/welcomedialog.cpp \
    tabstability.cpp \
    utils/stabilityutil.cpp

HEADERS += \
    pluginhandler.h \
    diffworker.h \
    mainwindow.h \
    intergrationsettingframe.h \
    coefswidget.h \
    coefitem.h \
    populationsetframe.h \
    resultwidget.h \
    plot2d.h \
    phaseportrait.h \
    commonplot.h \
    tabplot3d.h \
    plot3d.h \
    tabtable.h \
    tabledatamodel.h \
    external/exportdialog.h \
    external/dataloader.h \
    dialogs/resultdialog.h \
    dialogs/welcomedialog.h \
    tabstability.h \
    utils/stabilityutil.h

FORMS += \
    mainwindow.ui \
    intergrationsettingframe.ui \
    coefswidget.ui \
    coefitem.ui \
    populationsetframe.ui \
    resultwidget.ui \
    tabplot3d.ui \
    tabtable.ui \
    external/exportdialog.ui \
    dialogs/resultdialog.ui \
    dialogs/welcomedialog.ui \
    tabstability.ui

RESOURCES += \
    res.qrc

win32:RC_ICONS += assets/icon.ico
