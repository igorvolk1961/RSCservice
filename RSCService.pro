TARGET = RSCService
TEMPLATE = app
QT = core network xml gui
CONFIG += console

#GIS = GIS_OPERATOR
GIS = GIS_INTEGRATION

HEADERS += \
           src/include/global.h \
           src/include/myexception.h \
           src/include/serviceexceptionreport.h \
           src/include/try.h \
           src/requestmapper.h \
           src/controller/dumpcontroller.h \
           src/controller/rsccontroller.h

#           src/controller/fileuploadcontroller.h \
#           src/controller/sessioncontroller.h

SOURCES += \
           src/main.cpp \
           src/common/global.cpp \
           src/common/myexception.cpp \
           src/common/try.cpp \
           src/common/serviceexceptionreport.cpp \
           src/requestmapper.cpp \
           src/controller/dumpcontroller.cpp \
           src/controller/rsccontroller.cpp

#           src/controller/fileuploadcontroller.cpp \
#           src/controller/sessioncontroller.cpp

INCLUDEPATH += . \
           iml/include \
           src/include \
           src/gis_proxy

equals(GIS, GIS_OPERATOR){
   INCLUDEPATH += /usr/local/GISCore-operator12/include
   DEFINES += GIS_OPERATOR
}
equals(GIS, GIS_INTEGRATION){
   INCLUDEPATH += /usr/local/GISCore-9.20.0/include
}

# rsccomm собирается из исходников (iml/rsccomm) и кладётся в ./lib
LIBS += -L$$PWD/lib -lrsccomm

INCLUDEPATH += . \
               iml/rsccomm
OTHER_FILES += etc/* etc/ssl/* logs/* ../readme.txt

#---------------------------------------------------------------------------------------
# The following lines include the sources of the QtWebAppLib library
#---------------------------------------------------------------------------------------

include(../QtWebApp/logging/logging.pri)
include(../QtWebApp/httpserver/httpserver.pri)
# include(../QtWebApp/templateengine/templateengine.pri)
# Not used: include(../QtWebApp/qtservice/qtservice.pri)

#---------------------------------------------------------------------------------------


win32:QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__

#DEFINES += HIDEMAXMIN
#DEFINES += IGIS_LATE_LINK

DESTDIR = ./bin
OBJECTS_DIR = ./obj
MOC_DIR = ./moc

DISTFILES += \
    data/bin/SXFService.sh \
    data/bin/sxfserviced \
    data/bin/sxfserviced.service



