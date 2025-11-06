QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../../common.pri)
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

INCLUDEPATH+=$$INCLUDEDIR/wpsofficetool \
             $$INCLUDEDIR/ \
             $$INCLUDEDIR/common \
             $$INCLUDEDIR/wps \
             $$INCLUDEDIR/wpp \
             $$INCLUDEDIR/et

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

LIBS += -L"$$LIBDIR/quazipSelf" -lquazipSelf
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QMAKE_CXXFLAGS += -march=native
QMAKE_CFLAGS += -march=native
