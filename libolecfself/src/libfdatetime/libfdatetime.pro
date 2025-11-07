include(../publicArgs.pri)

DEFINES += HAVE_LOCAL_LIBFDATETIME

SOURCES += \
$$SRCDIR/libfdatetime_date_time_values.c \
$$SRCDIR/libfdatetime_error.c \
$$SRCDIR/libfdatetime_fat_date_time.c \
$$SRCDIR/libfdatetime_filetime.c \
$$SRCDIR/libfdatetime_floatingtime.c \
$$SRCDIR/libfdatetime_hfs_time.c \
$$SRCDIR/libfdatetime_nsf_timedate.c \
$$SRCDIR/libfdatetime_posix_time.c \
$$SRCDIR/libfdatetime_support.c \
$$SRCDIR/libfdatetime_systemtime.c

HEADERS += \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_date_time_values.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_definitions.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_error.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_extern.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_fat_date_time.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_filetime.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_floatingtime.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_hfs_time.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_libcerror.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_nsf_timedate.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_posix_time.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_support.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_systemtime.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_types.h \
$$SRCINCLUDEDIR/libfdatetime/libfdatetime_unused.h


INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$SRCINCLUDEDIR/libfdatetime
LIBS += -L"$$LIBDIR/libcerror" -llibcerror
