include(../../../common.pri)
include(../publicArgs.pri)

DEFINES += HAVE_LOCAL_LIBFWPS HAVE_LOCAL_LIBCNOTIFY HAVE_LOCAL_LIBCDATA HAVE_LOCAL_LIBFGUID HAVE_LOCAL_LIBUNA
HEADERS += \
$$SRCINCLUDEDIR/libfwps/libfwps_codepage.h \
$$SRCINCLUDEDIR/libfwps/libfwps_debug.h \
$$SRCINCLUDEDIR/libfwps/libfwps_definitions.h \
$$SRCINCLUDEDIR/libfwps/libfwps_error.h \
$$SRCINCLUDEDIR/libfwps/libfwps_extern.h \
$$SRCINCLUDEDIR/libfwps/libfwps_format_class_identifier.h \
$$SRCINCLUDEDIR/libfwps/libfwps_libcdata.h \
$$SRCINCLUDEDIR/libfwps/libfwps_libcerror.h \
$$SRCINCLUDEDIR/libfwps/libfwps_libcnotify.h \
$$SRCINCLUDEDIR/libfwps/libfwps_libfguid.h \
$$SRCINCLUDEDIR/libfwps/libfwps_libuna.h \
$$SRCINCLUDEDIR/libfwps/libfwps_notify.h \
$$SRCINCLUDEDIR/libfwps/libfwps_property_identifier.h \
$$SRCINCLUDEDIR/libfwps/libfwps_record.h \
$$SRCINCLUDEDIR/libfwps/libfwps_set.h \
$$SRCINCLUDEDIR/libfwps/libfwps_store.h \
$$SRCINCLUDEDIR/libfwps/libfwps_support.h \
$$SRCINCLUDEDIR/libfwps/libfwps_types.h \
$$SRCINCLUDEDIR/libfwps/libfwps_unused.h

SOURCES += \
$$SRCDIR/libfwps_debug.c \
$$SRCDIR/libfwps_error.c \
$$SRCDIR/libfwps_format_class_identifier.c \
$$SRCDIR/libfwps_notify.c \
$$SRCDIR/libfwps_property_identifier.c \
$$SRCDIR/libfwps_record.c \
$$SRCDIR/libfwps_set.c \
$$SRCDIR/libfwps_store.c \
$$SRCDIR/libfwps_support.c


INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$SRCINCLUDEDIR/libfwps
INCLUDEPATH += $$INCLUDEDIR/libcdata
INCLUDEPATH += $$SRCINCLUDEDIR/libcnotify
INCLUDEPATH += $$SRCINCLUDEDIR/libfguid
INCLUDEPATH += $$SRCINCLUDEDIR/libuna

LIBS += -L"$$LIBDIR/libuna" -llibuna
LIBS += -L"$$LIBDIR/libcnotify" -llibcnotify
LIBS += -L"$$LIBDIR/libfguid" -llibfguid
LIBS += -L"$$LIBDIR/libcdata" -llibcdata
LIBS += -L"$$LIBDIR/libcerror" -llibcerror
