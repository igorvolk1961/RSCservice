TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

QT += xml

QMAKE_CXXFLAGS += -O0
QMAKE_CFLAGS += -O0

SOURCES += \
    main.cpp

HEADERS +=

unix:INCLUDEPATH += . \
                    ../iml/include \
                    ../iml/rsccomm \
                    /usr/local/jdk/include \
                    /usr/local/jdk/include/linux

win32:INCLUDEPATH += . \
                    ../iml/include \
                    ../iml/rsccomm \
                    "c:\Program Files\Java\jdk1.8.0_152"/include \
                    "c:\Program Files\Java\jdk1.8.0_152"/include/win32

LIBS += -L../lib -lrsccomm

win32:QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__

DEFINES += HIDEMAXMIN

unix:LIBS += -ldl

OBJECTS_DIR = ../obj
DESTDIR = bin
