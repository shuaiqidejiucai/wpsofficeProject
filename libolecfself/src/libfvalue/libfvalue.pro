
include(../publicArgs.pri)

DEFINES += HAVE_LOCAL_LIBCNOTIFY HAVE_LOCAL_LIBUNA HAVE_LOCAL_LIBFVALUE HAVE_LOCAL_LIBCDATA
SOURCES += \
$$SRCDIR/libfvalue_binary_data.c \
$$SRCDIR/libfvalue_data_handle.c \
$$SRCDIR/libfvalue_error.c \
$$SRCDIR/libfvalue_filetime.c \
$$SRCDIR/libfvalue_floating_point.c \
$$SRCDIR/libfvalue_integer.c \
$$SRCDIR/libfvalue_split_utf16_string.c \
$$SRCDIR/libfvalue_split_utf8_string.c \
$$SRCDIR/libfvalue_string.c \
$$SRCDIR/libfvalue_support.c \
$$SRCDIR/libfvalue_table.c \
$$SRCDIR/libfvalue_utf16_string.c \
$$SRCDIR/libfvalue_utf8_string.c \
$$SRCDIR/libfvalue_value.c \
$$SRCDIR/libfvalue_value_entry.c \
$$SRCDIR/libfvalue_value_type.c

HEADERS += \
$$SRCINCLUDEDIR/libfvalue/libfvalue_binary_data.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_codepage.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_data_handle.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_definitions.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_error.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_extern.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_filetime.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_floating_point.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_integer.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_libcdata.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_libcerror.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_libcnotify.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_libfdatetime.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_libfguid.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_libfwnt.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_libuna.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_split_utf16_string.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_split_utf8_string.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_string.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_support.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_table.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_types.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_unused.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_utf16_string.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_utf8_string.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_value.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_value_entry.h \
$$SRCINCLUDEDIR/libfvalue/libfvalue_value_type.h


INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$SRCINCLUDEDIR/libfvalue
INCLUDEPATH += $$INCLUDEDIR/libcdata
INCLUDEPATH += $$SRCINCLUDEDIR/libcnotify
INCLUDEPATH += $$SRCINCLUDEDIR/libuna

LIBS += -L"$$LIBDIR/libuna" -llibuna
LIBS += -L"$$LIBDIR/libcnotify" -llibcnotify
LIBS += -L"$$LIBDIR/libcdata" -llibcdata
LIBS += -L"$$LIBDIR/libcerror" -llibcerror
