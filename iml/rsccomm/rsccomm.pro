TEMPLATE = lib
CONFIG += console c++11
CONFIG -= app_bundle

QT = xml

QMAKE_CXXFLAGS += -O0
QMAKE_CFLAGS += -O0

SOURCES += \
    ../imlcomm/imlline.cpp \
    ../imlcomm/imlsemn.cpp \
    ../imlcomm/imlserv.cpp \
    ../imlcomm/semdef.cpp \
    imlserv0.cpp \
    imlservex.cpp \
    rsc.cpp \
    rsctosvg.cpp \
    palette.cpp \
    postbuilder.cpp \
    converterhandler.cpp \
    myexception.cpp \
    semblockinfo.cpp \
    semutils1.cpp \
    imgtype.cpp \
    imgline.cpp \
    imgdot.cpp \
    imgsquare.cpp \
    imgvectortext.cpp \
    imgborder.cpp \
    imgpolygonglass.cpp \
    imgvectorex.cpp \
    imgvectormix.cpp \
    imgsection.cpp \
    imgmarkerline.cpp \
    imghatchsquare.cpp \
    imgpatternsquare.cpp \
    imgsquarevector.cpp \
    imglineglass.cpp \
    svgbuilderlibrary.cpp \
    svgbuildervectorex.cpp \
    svgbuilder.cpp \
    svgdesc.cpp \
    svgbuildervectormix.cpp \
    svgbuildervectorabstr.cpp \
    svgbuilderline.cpp \
    svgbuilderdrawabstr.cpp \
    svgbuilderdraw.cpp \
    svgbuilderdrawmix.cpp \
    svgbuilderdot.cpp \
    svgbuildertype.cpp \
    svgbuildersquare.cpp \
    svgbuilderpolygonglass.cpp \
    svgbuilderlineshift.cpp \
    svgbuilderdecorate.cpp \
    svgbuilderdash.cpp \
    svgbuildertext.cpp \
    svgbuilderhatchsquare.cpp \
    svgbuildervectornode.cpp \
    svgbuilderlineglass.cpp \
    svgbuildersquarevector.cpp \
    svgbuilderdotshift.cpp \
    utils.cpp \
    imgvectortextun.cpp

HEADERS += \
    imlserv0.h \
    imlservex.h \
    rscheader.h \
    rsc.h \
    rsctosvg.h \
    postbuilder.h \
    palette.h \
    targetver.h \
    myexception.h \
    semblockinfo.h \
    semutils1.h \
    converterhandler.h \
    debug.h \
    imgtype.h \
    imgline.h \
    imgdot.h \
    imgsquare.h \
    imgvectortext.h \
    imgborder.h \
    imgpolygonglass.h \
    imgvectorex.h \
    imgvectormix.h \
    imgsection.h \
    imgmarkerline.h \
    imghatchsquare.h \
    imgpatternsquare.h \
    imgsquarevector.h \
    imglineglass.h \
    svgbuildervectorex.h \
    svgbuilderlibrary.h \
    svgbuilder.h \
    svgdesc.h \
    svgbuildervectormix.h \
    svgbuildervectorabstr.h \
    svgbuilderline.h \
    svgbuilderdrawabstr.h \
    svgbuilderdraw.h \
    svgbuilderdrawmix.h \
    svgbuilderdot.h \
    svgbuildertype.h \
    svgbuildersquare.h \
    svgbuilderpolygonglass.h \
    svgbuilderlineshift.h \
    svgbuilderdecorate.h \
    svgbuilderdash.h \
    svgbuildertext.h \
    svgbuilderhatchsquare.h \
    svgbuildervectornode.h \
    svgbuilderlineglass.h \
    svgbuildersquarevector.h \
    svgbuilderdotshift.h \
    svgimage.h \
    imgvectortextun.h

win32:SOURCES += stdafx.cpp
win32:HEADERS += stdafx.h

unix:INCLUDEPATH += . \
                    ../include \
                    /usr/local/jdk/include \
                    /usr/local/jdk/include/linux \
                    /usr/include/glib-2.0 \
                    /usr/lib/x86_64-linux-gnu/glib-2.0/include \
                    /usr/include/freetype2

win32:INCLUDEPATH += . \
                    ../include \
                    "c:\Program Files\Java\jdk1.8.0_152"/include \
                    "c:\Program Files\Java\jdk1.8.0_152"/include/win32

win32:QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__

unix:LIBS += -ldl

DEFINES += HIDEMAXMIN
#DEFINES += SHORT_WCHAR
DEFINES += BUILD_IML

OBJECTS_DIR = ../../obj
DESTDIR = ../../lib
