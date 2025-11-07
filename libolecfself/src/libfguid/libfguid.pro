include(../publicArgs.pri)

DEFINES += HAVE_LOCAL_LIBFGUID
HEADERS += \
$$SRCINCLUDEDIR/libfguid/libfguid_definitions.h \
$$SRCINCLUDEDIR/libfguid/libfguid_error.h \
$$SRCINCLUDEDIR/libfguid/libfguid_extern.h \
$$SRCINCLUDEDIR/libfguid/libfguid_identifier.h \
$$SRCINCLUDEDIR/libfguid/libfguid_libcerror.h \
$$SRCINCLUDEDIR/libfguid/libfguid_support.h \
$$SRCINCLUDEDIR/libfguid/libfguid_types.h \
$$SRCINCLUDEDIR/libfguid/libfguid_unused.h

SOURCES += \
$$SRCDIR/libfguid_error.c \
$$SRCDIR/libfguid_identifier.c \
$$SRCDIR/libfguid_support.c

INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$SRCINCLUDEDIR/libfguid
LIBS += -L"$$LIBDIR/libcerror" -llibcerror
