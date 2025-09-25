QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++0x -Wno-attributes
TARGET = wpsDemo
TEMPLATE = app
QT_QPA_PLATFORM=xcb
DEFINES+=LIBOLECF_HAVE_WIDE_CHARACTER_TYPE HAVE_LOCAL_LIBBFIO HAVE_LOCAL_LIBCDATA HAVE_LOCAL_LIBCERROR LIBOLECF_HAVE_BFIO
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
                ../include/common \
                ../include/wps \
                ../include/wpp \
                ../include/et

SOURCES += \
    etcomment.cpp \
    main.cpp \
    mainwindow.cpp \
    wppcomment.cpp \
    wpscomment.cpp \
    wpsmanager.cpp

HEADERS += \
    etcomment.h \
    mainwindow.h \
    wppcomment.h \
    wpscomment.h \
    wpsmanager.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += /usr/local/include/QuaZip-Qt5-1.5/quazip
LIBS += -L"/usr/local/lib/" -lquazip1-qt5

# INCLUDEPATH += /usr/local/include/libolecf

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /home/user/mjc/libolecf/common
INCLUDEPATH += /home/user/mjc/libolecf/libbfio
INCLUDEPATH += /home/user/mjc/libolecf/libcdata
INCLUDEPATH += /home/user/mjc/libolecf/libcerror
INCLUDEPATH += /home/user/mjc/libolecf/libolecf
LIBS += -L"/usr/local/lib" -lolecf

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
