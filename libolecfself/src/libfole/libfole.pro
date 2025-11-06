include(../../../common.pri)
include(../publicArgs.pri)

DEFINES += HAVE_LOCAL_LIBFOLE
SOURCES += \
$$SRCDIR/libfole_error.c \
$$SRCDIR/libfole_support.c \
$$SRCDIR/libfole_value_type.c

HEADERS += \
$$SRCINCLUDEDIR/libfole/libfole_definitions.h \
$$SRCINCLUDEDIR/libfole/libfole_error.h \
$$SRCINCLUDEDIR/libfole/libfole_extern.h \
$$SRCINCLUDEDIR/libfole/libfole_libcerror.h \
$$SRCINCLUDEDIR/libfole/libfole_support.h \
$$SRCINCLUDEDIR/libfole/libfole_types.h \
$$SRCINCLUDEDIR/libfole/libfole_unused.h \
$$SRCINCLUDEDIR/libfole/libfole_value_type.h

INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$SRCINCLUDEDIR/libfole
LIBS += -L"$$LIBDIR/libcerror" -llibcerror
