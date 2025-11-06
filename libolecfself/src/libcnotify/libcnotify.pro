include(../../../common.pri)
include(../publicArgs.pri)

DEFINES += HAVE_ERRNO_H HAVE_STDARG_H HAVE_LOCAL_LIBCNOTIFY

SOURCES += \
$$SRCDIR/libcnotify_print.c \
$$SRCDIR/libcnotify_stream.c \
$$SRCDIR/libcnotify_support.c \
$$SRCDIR/libcnotify_verbose.c

HEADERS += \
$$SRCINCLUDEDIR/libcnotify/libcnotify_definitions.h \
$$SRCINCLUDEDIR/libcnotify/libcnotify_extern.h \
$$SRCINCLUDEDIR/libcnotify/libcnotify_libcerror.h \
$$SRCINCLUDEDIR/libcnotify/libcnotify_print.h \
$$SRCINCLUDEDIR/libcnotify/libcnotify_stream.h \
$$SRCINCLUDEDIR/libcnotify/libcnotify_support.h \
$$SRCINCLUDEDIR/libcnotify/libcnotify_unused.h \
$$SRCINCLUDEDIR/libcnotify/libcnotify_verbose.h

INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$SRCINCLUDEDIR/libcnotify
INCLUDEPATH += $$INCLUDEDIR/libcerror

LIBS += -L"$$LIBDIR/libcerror" -llibcerror

