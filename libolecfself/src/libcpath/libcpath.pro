include(../../../common.pri)
include(../publicArgs.pri)

DEFINES += HAVE_UNISTD_H HAVE_SYS_STAT_H HAVE_ERRNO_H HAVE_LIMITS_H HAVE_GETCWD HAVE_LOCAL_LIBUNA HAVE_LOCAL_LIBCSPLIT HAVE_LOCAL_LIBCLOCALE HAVE_LOCAL_LIBCPATH
HEADERS += \
$$SRCINCLUDEDIR/libcpath/libcpath_definitions.h \
$$SRCINCLUDEDIR/libcpath/libcpath_error.h \
$$SRCINCLUDEDIR/libcpath/libcpath_extern.h \
$$SRCINCLUDEDIR/libcpath/libcpath_libcerror.h \
$$SRCINCLUDEDIR/libcpath/libcpath_libclocale.h \
$$SRCINCLUDEDIR/libcpath/libcpath_libcsplit.h \
$$SRCINCLUDEDIR/libcpath/libcpath_libuna.h \
$$SRCINCLUDEDIR/libcpath/libcpath_path.h \
$$SRCINCLUDEDIR/libcpath/libcpath_support.h \
$$SRCINCLUDEDIR/libcpath/libcpath_system_string.h \
$$SRCINCLUDEDIR/libcpath/libcpath_unused.h

SOURCES += \
$$SRCDIR/libcpath_error.c \
$$SRCDIR/libcpath_path.c \
$$SRCDIR/libcpath_support.c \
$$SRCDIR/libcpath_system_string.c

INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$SRCINCLUDEDIR/libcpath
INCLUDEPATH += $$INCLUDEDIR/libcerror
INCLUDEPATH += $$SRCINCLUDEDIR/libclocale
INCLUDEPATH += $$SRCINCLUDEDIR/libuna
INCLUDEPATH += $$SRCINCLUDEDIR/libcsplit

LIBS += -L"$$LIBDIR/libuna" -llibuna
LIBS += -L"$$LIBDIR/libcerror" -llibcerror
LIBS += -L"$$LIBDIR/libclocale" -llibclocale
LIBS += -L"$$LIBDIR/libcsplit" -llibcsplit
