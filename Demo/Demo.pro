QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

include(../qpipedevice.pri)

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp
