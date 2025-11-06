include(../../../common.pri)
include(../publicArgs.pri)
DEFINES += HAVE_LOCAL_LIBCLOCALE HAVE_LOCAL_LIBFVALUE HAVE_LOCAL_LIBFWPS HAVE_LOCAL_LIBFOLE HAVE_LOCAL_LIBFGUID HAVE_LOCAL_LIBUNA HAVE_LOCAL_LIBFDATETIME HAVE_LOCAL_LIBCDATA HAVE_LOCAL_LIBBFIO HAVE_LOCAL_LIBCNOTIFY
HEADERS += \
$$INCLUDEDIR/libolecf/libolecf.h \
$$INCLUDEDIR/libolecf/libolecf/codepage.h \
$$INCLUDEDIR/libolecf/libolecf/definitions.h \
$$INCLUDEDIR/libolecf/libolecf/error.h \
$$INCLUDEDIR/libolecf/libolecf/extern.h \
$$INCLUDEDIR/libolecf/libolecf/features.h \
$$INCLUDEDIR/libolecf/libolecf/ole.h \
$$INCLUDEDIR/libolecf/libolecf/types.h \
$$INCLUDEDIR/libolecf/libolecf_allocation_table.h \
$$INCLUDEDIR/libolecf/libolecf_codepage.h \
$$INCLUDEDIR/libolecf/libolecf_compound_object_stream.h \
$$INCLUDEDIR/libolecf/libolecf_debug.h \
$$INCLUDEDIR/libolecf/libolecf_definitions.h \
$$INCLUDEDIR/libolecf/libolecf_directory_entry.h \
$$INCLUDEDIR/libolecf/libolecf_directory_list.h \
$$INCLUDEDIR/libolecf/libolecf_directory_tree.h \
$$INCLUDEDIR/libolecf/libolecf_error.h \
$$INCLUDEDIR/libolecf/libolecf_extern.h \
$$INCLUDEDIR/libolecf/libolecf_file.h \
$$INCLUDEDIR/libolecf/libolecf_file_header.h \
$$INCLUDEDIR/libolecf/libolecf_io_handle.h \
$$INCLUDEDIR/libolecf/libolecf_item.h \
$$INCLUDEDIR/libolecf/libolecf_libbfio.h \
$$INCLUDEDIR/libolecf/libolecf_libcdata.h \
$$INCLUDEDIR/libolecf/libolecf_libcerror.h \
$$INCLUDEDIR/libolecf/libolecf_libclocale.h \
$$INCLUDEDIR/libolecf/libolecf_libcnotify.h \
$$INCLUDEDIR/libolecf/libolecf_libfdatetime.h \
$$INCLUDEDIR/libolecf/libolecf_libfguid.h \
$$INCLUDEDIR/libolecf/libolecf_libfole.h \
$$INCLUDEDIR/libolecf/libolecf_libfvalue.h \
$$INCLUDEDIR/libolecf/libolecf_libfwps.h \
$$INCLUDEDIR/libolecf/libolecf_libuna.h \
$$INCLUDEDIR/libolecf/libolecf_notify.h \
$$INCLUDEDIR/libolecf/libolecf_ole.h \
$$INCLUDEDIR/libolecf/libolecf_property_section.h \
$$INCLUDEDIR/libolecf/libolecf_property_set.h \
$$INCLUDEDIR/libolecf/libolecf_property_set_stream.h \
$$INCLUDEDIR/libolecf/libolecf_property_value.h \
$$INCLUDEDIR/libolecf/libolecf_stream.h \
$$INCLUDEDIR/libolecf/libolecf_support.h \
$$INCLUDEDIR/libolecf/libolecf_types.h \
$$INCLUDEDIR/libolecf/libolecf_unused.h \
$$INCLUDEDIR/libolecf/olecf_compound_object.h \
$$INCLUDEDIR/libolecf/olecf_directory.h \
$$INCLUDEDIR/libolecf/olecf_file_header.h \
$$INCLUDEDIR/libolecf/olecf_property_set.h

SOURCES += \
$$SRCDIR/libolecf.c \
$$SRCDIR/libolecf_allocation_table.c \
$$SRCDIR/libolecf_compound_object_stream.c \
$$SRCDIR/libolecf_debug.c \
$$SRCDIR/libolecf_directory_entry.c \
$$SRCDIR/libolecf_directory_list.c \
$$SRCDIR/libolecf_directory_tree.c \
$$SRCDIR/libolecf_error.c \
$$SRCDIR/libolecf_file.c \
$$SRCDIR/libolecf_file_header.c \
$$SRCDIR/libolecf_io_handle.c \
$$SRCDIR/libolecf_item.c \
$$SRCDIR/libolecf_notify.c \
$$SRCDIR/libolecf_property_section.c \
$$SRCDIR/libolecf_property_set.c \
$$SRCDIR/libolecf_property_set_stream.c \
$$SRCDIR/libolecf_property_value.c \
$$SRCDIR/libolecf_stream.c \
$$SRCDIR/libolecf_support.c



INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$INCLUDEDIR/libolecf
INCLUDEPATH += $$SRCINCLUDEDIR/libfwps
INCLUDEPATH += $$INCLUDEDIR/libbfio
INCLUDEPATH += $$INCLUDEDIR/libcdata
INCLUDEPATH += $$SRCINCLUDEDIR/libcnotify
INCLUDEPATH += $$SRCINCLUDEDIR/libfdatetime
INCLUDEPATH += $$SRCINCLUDEDIR/libfguid
INCLUDEPATH += $$SRCINCLUDEDIR/libfole
INCLUDEPATH += $$SRCINCLUDEDIR/libfvalue
INCLUDEPATH += $$SRCINCLUDEDIR/libuna
INCLUDEPATH += $$SRCINCLUDEDIR/libclocale

LIBS += -L"$$LIBDIR/libcerror" -llibcerror
LIBS += -L"$$LIBDIR/libcdata" -llibcdata
LIBS += -L"$$LIBDIR/libfwps" -llibfwps
LIBS += -L"$$LIBDIR/libbfio" -llibbfio
LIBS += -L"$$LIBDIR/libcnotify" -llibcnotify
LIBS += -L"$$LIBDIR/libfdatetime" -llibfdatetime
LIBS += -L"$$LIBDIR/libfguid" -llibfguid
LIBS += -L"$$LIBDIR/libfole" -llibfole
LIBS += -L"$$LIBDIR/libfvalue" -llibfvalue
LIBS += -L"$$LIBDIR/libuna" -llibuna
LIBS += -L"$$LIBDIR/libclocale" -llibclocale
