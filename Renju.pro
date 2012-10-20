#-------------------------------------------------
#
# Project created by QtCreator 2011-06-21T09:00:22
#
#-------------------------------------------------

QT       += core gui network

TARGET = Renju
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    configuration.cpp \
    evaluate.cpp \
    peopvscomp.cpp \
    peopvspeop.cpp \
    search.cpp \
    buildserver.cpp \
    tcpserver.cpp \
    findserver.cpp

HEADERS  += widget.h \
    configuration.h \
    evaluate.h \
    peopvscomp.h \
    peopvspeop.h \
    search.h \
    buildserver.h \
    tcpserver.h \
    findserver.h \
    my_stl.h

FORMS    += widget.ui

RESOURCES += \
    sound.qrc \
    image.qrc
