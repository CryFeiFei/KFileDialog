#-------------------------------------------------
#
# Project created by QtCreator 2019-07-11T22:11:56
#
#-------------------------------------------------

QT += core gui
QT += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KFileDialog
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

CONFIG += c++11

SOURCES += \
        kfile/kfile.cpp \
        kfiledialog.cpp \
        kfileitemmodel.cpp \
        kfileitemnode.cpp \
        kstackwidget.cpp \
        main.cpp \
        mainwindow.cpp \
        modelview/kdetailview.cpp \
        thread/klocalloadthread.cpp \
        watcher/kfilesystemwatcher.cpp

HEADERS += \
        global.h \
        kfile/kfile.h \
        kfiledialog.h \
        kfileitemmodel.h \
        kfileitemnode.h \
        kstackwidget.h \
        mainwindow.h \
        modelview/kdetailview.h \
        thread/klocalloadthread.h \
        watcher/kfilesystemwatcher.h

FORMS += \
        kfiledialog.ui \
        mainwindow.ui

