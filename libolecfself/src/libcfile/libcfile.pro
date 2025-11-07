
include(../publicArgs.pri)

DEFINES += HAVE_UNLINK HAVE_IOCTL HAVE_WRITE HAVE_READ HAVE_FSTAT HAVE_FCNTL_H HAVE_OPEN HAVE_SYS_STAT_H HAVE_STAT HAVE_LSEEK HAVE_ERRNO_H HAVE_CLOSE HAVE_LOCAL_LIBUNA HAVE_LOCAL_LIBCNOTIFY HAVE_LOCAL_LIBCLOCALE HAVE_LOCAL_LIBCFILE
SOURCES += \
$$SRCDIR/libcfile_error.c \
$$SRCDIR/libcfile_file.c \
$$SRCDIR/libcfile_notify.c \
$$SRCDIR/libcfile_support.c \
$$SRCDIR/libcfile_system_string.c \
$$SRCDIR/libcfile_winapi.c

HEADERS += \
$$SRCINCLUDEDIR/libcfile/libcfile_definitions.h \
$$SRCINCLUDEDIR/libcfile/libcfile_error.h \
$$SRCINCLUDEDIR/libcfile/libcfile_extern.h \
$$SRCINCLUDEDIR/libcfile/libcfile_file.h \
$$SRCINCLUDEDIR/libcfile/libcfile_libcerror.h \
$$SRCINCLUDEDIR/libcfile/libcfile_libclocale.h \
$$SRCINCLUDEDIR/libcfile/libcfile_libcnotify.h \
$$SRCINCLUDEDIR/libcfile/libcfile_libuna.h \
$$SRCINCLUDEDIR/libcfile/libcfile_notify.h \
$$SRCINCLUDEDIR/libcfile/libcfile_support.h \
$$SRCINCLUDEDIR/libcfile/libcfile_system_string.h \
$$SRCINCLUDEDIR/libcfile/libcfile_types.h \
$$SRCINCLUDEDIR/libcfile/libcfile_unused.h \
$$SRCINCLUDEDIR/libcfile/libcfile_winapi.h


INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$SRCINCLUDEDIR/libcfile
INCLUDEPATH += $$INCLUDEDIR/libcerror
INCLUDEPATH += $$SRCINCLUDEDIR/libclocale
INCLUDEPATH += $$SRCINCLUDEDIR/libuna
INCLUDEPATH += $$SRCINCLUDEDIR/libcnotify


LIBS += -L"$$LIBDIR/libcerror" -llibcerror
LIBS += -L"$$LIBDIR/libuna" -llibuna
LIBS += -L"$$LIBDIR/libcnotify" -llibcnotify
LIBS += -L"$$LIBDIR/libclocale" -llibclocale
