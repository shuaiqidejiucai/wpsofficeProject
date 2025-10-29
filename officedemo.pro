QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++0x -Wno-attributes
TARGET = wpsDemo
TEMPLATE = app
QT_QPA_PLATFORM=xcb

DEFINES+= HAVE_LOCAL_LIBBFIO HAVE_LOCAL_LIBCDATA HAVE_LOCAL_LIBCERROR
exists(/opt/kingsoft/wps-office/office6/libstdc++.so.6){
        system(ln -s /opt/kingsoft/wps-office/office6/libstdc++.so.6  libstdc++.so.6)
        LIBS += libstdc++.so.6
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
    main.cpp \
    mainwindow.cpp \
    officegeneralabilities.cpp \
    utilitytool.cpp \
    wppcomment.cpp \
    wpscomment.cpp

HEADERS += \
    etcomment.h \
    mainwindow.h \
    officegeneralabilities.h \
    utilitytool.h \
    wppcomment.h \
    wpscomment.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += $$PWD/qunzip/include
LIBS += -L"$$PWD/qunzip/lib" -lquazipSelf


INCLUDEPATH += $$PWD/libolecfself/include
INCLUDEPATH += $$PWD/libolecfself/include/common
INCLUDEPATH += $$PWD/libolecfself/include/libbfio
INCLUDEPATH += $$PWD/libolecfself/include/libcdata
INCLUDEPATH += $$PWD/libolecfself/include/libcerror
INCLUDEPATH += $$PWD/libolecfself/include/libolecf
INCLUDEPATH += $$PWD/libolecfself/src/include/libcpath

INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$INCLUDEDIR/libolecf
INCLUDEPATH += $$SRCINCLUDEDIR/libfwps
INCLUDEPATH += $$INCLUDEDIR/libbfio

INCLUDEPATH += $$INCLUDEDIR/libcdata
INCLUDEPATH += $$SRCINCLUDEDIR/libcnotify
INCLUDEPATH += $$SRCINCLUDEDIR/libfdatetime
INCLUDEPATH += $$SRCINCLUDEDIR/libfguid
INCLUDEPATH += $$SRCINCLUDEDIR/libfole
INCLUDEPATH += $$SRCINCLUDEDIR/libfvalue
INCLUDEPATH += $$SRCINCLUDEDIR/libcfile
INCLUDEPATH += $$SRCINCLUDEDIR/libuna
# INCLUDEPATH += $$SRCINCLUDEDIR/libcpath
INCLUDEPATH += $$SRCINCLUDEDIR/libclocale


LIBS += -L"$$PWD/libolecfself/lib/libolecf" -llibolecf
LIBS += -L"$$PWD/libolecfself/lib/libcdata" -llibcdata
LIBS += -L"$$PWD/libolecfself/lib/libfwps" -llibfwps
LIBS += -L"$$PWD/libolecfself/lib/libbfio" -llibbfio
LIBS += -L"$$PWD/libolecfself/lib/libcnotify" -llibcnotify
LIBS += -L"$$PWD/libolecfself/lib/libfdatetime" -llibfdatetime
LIBS += -L"$$PWD/libolecfself/lib/libfguid" -llibfguid
LIBS += -L"$$PWD/libolecfself/lib/libfole" -llibfole
LIBS += -L"$$PWD/libolecfself/lib/libfvalue" -llibfvalue
LIBS += -L"$$PWD/libolecfself/lib/libuna" -llibuna
LIBS += -L"$$PWD/libolecfself/lib/libclocale" -llibclocale
LIBS += -L"$$PWD/libolecfself/lib/libcpath" -llibcpath
LIBS += -L"$$PWD/libolecfself/lib/libcfile" -llibcfile
LIBS += -L"$$PWD/libolecfself/lib/libcsplit" -llibcsplit
LIBS += -L"$$PWD/libolecfself/lib/libcerror" -llibcerror
# win32 {
#     QMAKE_PRE_LINK += copy /Y $$PWD/lib/source.txt $$PWD/lib//libquazipSelf.txt
# } else:unix {
#     QMAKE_PRE_LINK += cp -f $$PWD/libolecfself/lib/ $$OUT_PWD/lib/
# }

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
