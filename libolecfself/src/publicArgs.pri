TEMPLATE = lib
CONFIG += staticlib
# GLIBINCLUDEPATH=/usr/include/glib-2.0/
CONFIG += c++11
DEFINES += HAVE_VSNPRINTF HAVE_STRLEN HAVE_MALLOC HAVE_MEMSET HAVE_MEMCPY HAVE_MEMCMP HAVE_REALLOC HAVE_FREE HAVE_STDLIB_H HAVE_STRING_H  HAVE_LOCAL_LIBCERROR
INCLUDEDIR = $$PWD/../include
SRCDIR = $$PWD/$$TARGET
SRCINCLUDEDIR = $$PWD/include
LIBDIR = $$PWD/../lib
INCLUDEPATH += $$INCLUDEDIR/libcerror

HEADERS += \
    $$INCLUDEDIR/common/byte_stream.h \
    $$INCLUDEDIR/common/config.h \
    $$INCLUDEDIR/common/file_stream.h \
    $$INCLUDEDIR/common/system_string.h \
    $$INCLUDEDIR/common/common.h \
    $$INCLUDEDIR/common/config_msc.h \
    $$INCLUDEDIR/common/memory.h \
    $$INCLUDEDIR/common/types.h \
    $$INCLUDEDIR/common/config_borlandc.h \
    $$INCLUDEDIR/common/config_winapi.h \
    $$INCLUDEDIR/common/narrow_string.h \
    $$INCLUDEDIR/common/wide_string.h


OBJECTS_DIR = $$PWD/../build/$$TARGET/obj
MOC_DIR = $$PWD/../build/$$TARGET/moc
UI_DIR = $$PWD/../build/$$TARGET/ui
RCC_DIR = $$PWD/../build/$$TARGET/rcc
DESTDIR = $$PWD/../lib/$$TARGET
