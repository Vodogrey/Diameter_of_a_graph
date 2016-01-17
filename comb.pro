QT += core gui widgets printsupport



TARGET = comb
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    MainWindow.cpp \
    mathgraph.cpp \
    qcustomplot.cpp

HEADERS += \
    MainWindow.h \
    mathgraph.h \
    qcustomplot.h

