QT -= gui

TEMPLATE = lib
DEFINES += QUAZIP_BUILD
#CONFIG += staticlib
CONFIG += c++11

include(../common.pri)
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/JlCompress.cpp \
    src/qioapi.cpp \
    src/quaadler32.cpp \
    src/quachecksum32.cpp \
    src/quacrc32.cpp \
    src/quagzipfile.cpp \
    src/quaziodevice.cpp \
    src/quazip.cpp \
    src/quazipdir.cpp \
    src/quazipfile.cpp \
    src/quazipfileinfo.cpp \
    src/quazipnewinfo.cpp \
    src/unzip.c \
    src/zip.c

HEADERS += \
    include/JlCompress.h \
    include/ioapi.h \
    include/minizip_crypt.h \
    include/quaadler32.h \
    include/quachecksum32.h \
    include/quacrc32.h \
    include/quagzipfile.h \
    include/quaziodevice.h \
    include/quazip.h \
    include/quazip_global.h \
    include/quazip_qt_compat.h \
    include/quazipdir.h \
    include/quazipfile.h \
    include/quazipfileinfo.h \
    include/quazipnewinfo.h \
    include/unzip.h \
    include/zip.h






INCLUDEPATH+=$$PWD/include
# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

