include(../publicArgs.pri)

DEFINES += HAVE_LOCALE_H HAVE_LOCAL_LIBCLOCALE

SOURCES += \
$$SRCDIR/libclocale_codepage.c \
$$SRCDIR/libclocale_locale.c \
$$SRCDIR/libclocale_support.c \
$$SRCDIR/libclocale_wide_string.c

HEADERS += \
$$SRCINCLUDEDIR/libclocale/libclocale_codepage.h \
$$SRCINCLUDEDIR/libclocale/libclocale_definitions.h \
$$SRCINCLUDEDIR/libclocale/libclocale_extern.h \
$$SRCINCLUDEDIR/libclocale/libclocale_libcerror.h \
$$SRCINCLUDEDIR/libclocale/libclocale_locale.h \
$$SRCINCLUDEDIR/libclocale/libclocale_support.h \
$$SRCINCLUDEDIR/libclocale/libclocale_unused.h \
$$SRCINCLUDEDIR/libclocale/libclocale_wide_string.h

INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$SRCINCLUDEDIR/libclocale
INCLUDEPATH += $$INCLUDEDIR/libcerror

LIBS += -L"$$LIBDIR/libcerror" -llibcerror
