include(../../../common.pri)
include(../publicArgs.pri)

DEFINES += HAVE_STRERROR HAVE_STDARG_H

SOURCES += \
$$SRCDIR/libcerror_error.c \
$$SRCDIR/libcerror_support.c \
$$SRCDIR/libcerror_system.c

HEADERS += \
$$INCLUDEDIR/libcerror/libcerror_definitions.h \
$$INCLUDEDIR/libcerror/libcerror_error.h \
$$INCLUDEDIR/libcerror/libcerror_extern.h \
$$INCLUDEDIR/libcerror/libcerror_support.h \
$$INCLUDEDIR/libcerror/libcerror_system.h \
$$INCLUDEDIR/libcerror/libcerror_types.h \
$$INCLUDEDIR/libcerror/libcerror_unused.h

INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$INCLUDEDIR/libcerror
