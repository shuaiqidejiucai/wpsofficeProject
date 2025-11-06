QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++0x -Wno-attributes
TARGET = pptcfunoutout
TEMPLATE = lib
CONFIG += dll
QT_QPA_PLATFORM=xcb

include(common.pri)
DEFINES+= HAVE_LOCAL_LIBBFIO HAVE_LOCAL_LIBCDATA HAVE_LOCAL_LIBCERROR

exists(/opt/kingsoft/wps-office/office6/libstdc++.so.6){
        system(ln -s /opt/kingsoft/wps-office/office6/libstdc++.so.6  libstdc++.so.6)
        LIBS += $$PWD/libstdc++.so.6
}

QMAKE_LFLAGS += -Wl,--rpath=\'\$\$ORIGIN\':$$[QT_INSTALL_LIBS]:/opt/kingsoft/wps-office/office6
QMAKE_LIBDIR =  ./ $$[QT_INSTALL_LIBS]  /opt/kingsoft/wps-office/office6

greaterThan(QT_MAJOR_VERSION, 4){
        LIBS += -lrpcwpsapi_sysqt5 -lrpcetapi_sysqt5 -lrpcwppapi_sysqt5
        exists(/opt/kingsoft/wps-office/office6/libc++abi.so.1){
                system(ln -sf /opt/kingsoft/wps-office/office6/libc++abi.so.1  libc++abi.so.1)
                LIBS += libc++abi.so.1
        }
}
else{
        LIBS += -lrpcwpsapi -lrpcetapi -lrpcwppapi
}


INCLUDEPATH = . \
                ./et \
                ./wps \
                ./wpp \
                ./include/common \
                ./include/wps \
                ./include/wpp \
                ./include/et

SOURCES += \
    etcomment.cpp \
    officegeneralabilities.cpp \
    pptcfunoutput.cpp \
    utilitytool.cpp \
    wppcomment.cpp \
    wpscomment.cpp

HEADERS += \
    include/wpsofficetool/etcomment.h \
    include/wpsofficetool/officegeneralabilities.h \
    include/wpsofficetool/pptcfunoutput.h \
    include/wpsofficetool/utilitytool.h \
    include/wpsofficetool/wppcomment.h \
    include/wpsofficetool/wpscomment.h

INCLUDEPATH += $$PWD/qunzip/include

INCLUDEPATH += $$PWD/include/wpsofficetool

INCLUDEPATH += $$PWD/libolecfself/include
INCLUDEPATH += $$PWD/libolecfself/include/common
INCLUDEPATH += $$PWD/libolecfself/include/libbfio
INCLUDEPATH += $$PWD/libolecfself/include/libcdata
INCLUDEPATH += $$PWD/libolecfself/include/libcerror
INCLUDEPATH += $$PWD/libolecfself/include/libolecf
INCLUDEPATH += $$PWD/libolecfself/src/include/libcpath

#INCLUDEPATH += $$INCLUDEDIR/common
#INCLUDEPATH += $$INCLUDEDIR/libolecf
#INCLUDEPATH += $$SRCINCLUDEDIR/libfwps
#INCLUDEPATH += $$INCLUDEDIR/libbfio

#INCLUDEPATH += $$INCLUDEDIR/libcdata
#INCLUDEPATH += $$SRCINCLUDEDIR/libcnotify
#INCLUDEPATH += $$SRCINCLUDEDIR/libfdatetime
#INCLUDEPATH += $$SRCINCLUDEDIR/libfguid
#INCLUDEPATH += $$SRCINCLUDEDIR/libfole
#INCLUDEPATH += $$SRCINCLUDEDIR/libfvalue
#INCLUDEPATH += $$SRCINCLUDEDIR/libcfile
#INCLUDEPATH += $$SRCINCLUDEDIR/libuna
## INCLUDEPATH += $$SRCINCLUDEDIR/libcpath
#INCLUDEPATH += $$SRCINCLUDEDIR/libclocale




LIBS += -L"$$LIBDIR/quazipSelf" -lquazipSelf
LIBS += -L"$$LIBDIR/libolecf" -llibolecf
LIBS += -L"$$LIBDIR/libcdata" -llibcdata
LIBS += -L"$$LIBDIR/libfwps" -llibfwps
LIBS += -L"$$LIBDIR/libbfio" -llibbfio
LIBS += -L"$$LIBDIR/libcnotify" -llibcnotify
LIBS += -L"$$LIBDIR/libfdatetime" -llibfdatetime
LIBS += -L"$$LIBDIR/libfguid" -llibfguid
LIBS += -L"$$LIBDIR/libfole" -llibfole
LIBS += -L"$$LIBDIR/libfvalue" -llibfvalue
LIBS += -L"$$LIBDIR/libuna" -llibuna
LIBS += -L"$$LIBDIR/libclocale" -llibclocale
LIBS += -L"$$LIBDIR/libcpath" -llibcpath
LIBS += -L"$$LIBDIR/libcfile" -llibcfile
LIBS += -L"$$LIBDIR/libcsplit" -llibcsplit
LIBS += -L"$$LIBDIR/libcerror" -llibcerror
# win32 {
#     QMAKE_PRE_LINK += copy /Y $$PWD/lib/source.txt $$PWD/lib//libquazipSelf.txt
# } else:unix {
#     QMAKE_PRE_LINK += cp -f $$DESTDIR/../ $$OUT_PWD/lib/
# }

# Default rules for deployment.
#OBJECTS_DIR = $$PWD/build/obj
#MOC_DIR = $$PWD/build/moc
#UI_DIR = $$PWD/build/ui
#RCC_DIR = $$PWD/build/rcc
#DESTDIR = $$PWD/lib
QMAKE_CXXFLAGS += -march=native
QMAKE_CFLAGS += -march=native
