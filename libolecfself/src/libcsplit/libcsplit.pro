include(../publicArgs.pri)

DEFINES += HAVE_LOCAL_LIBCSPLIT

SOURCES += \
$$SRCDIR/libcsplit_error.c \
$$SRCDIR/libcsplit_narrow_split_string.c \
$$SRCDIR/libcsplit_narrow_string.c \
$$SRCDIR/libcsplit_support.c \
$$SRCDIR/libcsplit_wide_split_string.c \
$$SRCDIR/libcsplit_wide_string.c

HEADERS += \
$$SRCINCLUDEDIR/libcsplit/libcsplit_definitions.h \
$$SRCINCLUDEDIR/libcsplit/libcsplit_error.h \
$$SRCINCLUDEDIR/libcsplit/libcsplit_extern.h \
$$SRCINCLUDEDIR/libcsplit/libcsplit_libcerror.h \
$$SRCINCLUDEDIR/libcsplit/libcsplit_narrow_split_string.h \
$$SRCINCLUDEDIR/libcsplit/libcsplit_narrow_string.h \
$$SRCINCLUDEDIR/libcsplit/libcsplit_support.h \
$$SRCINCLUDEDIR/libcsplit/libcsplit_types.h \
$$SRCINCLUDEDIR/libcsplit/libcsplit_unused.h \
$$SRCINCLUDEDIR/libcsplit/libcsplit_wide_split_string.h \
$$SRCINCLUDEDIR/libcsplit/libcsplit_wide_string.h

INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$SRCINCLUDEDIR/libcsplit
INCLUDEPATH += $$INCLUDEDIR/libcerror

LIBS += -L"$$LIBDIR/libcerror" -llibcerror
