
include(../publicArgs.pri)

DEFINES += HAVE_LOCAL_LIBCPATH HAVE_LOCAL_LIBUNA HAVE_LOCAL_LIBCDATA HAVE_LOCAL_LIBCLOCALE HAVE_LOCAL_LIBCFILE HAVE_LOCAL_LIBBFIO
SOURCES += \
$$SRCDIR/libbfio_error.c \
$$SRCDIR/libbfio_file.c \
$$SRCDIR/libbfio_file_io_handle.c \
$$SRCDIR/libbfio_file_pool.c \
$$SRCDIR/libbfio_file_range.c \
$$SRCDIR/libbfio_file_range_io_handle.c \
$$SRCDIR/libbfio_handle.c \
$$SRCDIR/libbfio_memory_range.c \
$$SRCDIR/libbfio_memory_range_io_handle.c \
$$SRCDIR/libbfio_pool.c \
$$SRCDIR/libbfio_support.c \
$$SRCDIR/libbfio_system_string.c

HEADERS += \
$$INCLUDEDIR/libbfio/libbfio_codepage.h \
$$INCLUDEDIR/libbfio/libbfio_definitions.h \
$$INCLUDEDIR/libbfio/libbfio_error.h \
$$INCLUDEDIR/libbfio/libbfio_extern.h \
$$INCLUDEDIR/libbfio/libbfio_file.h \
$$INCLUDEDIR/libbfio/libbfio_file_io_handle.h \
$$INCLUDEDIR/libbfio/libbfio_file_pool.h \
$$INCLUDEDIR/libbfio/libbfio_file_range.h \
$$INCLUDEDIR/libbfio/libbfio_file_range_io_handle.h \
$$INCLUDEDIR/libbfio/libbfio_handle.h \
$$INCLUDEDIR/libbfio/libbfio_libcdata.h \
$$INCLUDEDIR/libbfio/libbfio_libcerror.h \
$$INCLUDEDIR/libbfio/libbfio_libcfile.h \
$$INCLUDEDIR/libbfio/libbfio_libclocale.h \
$$INCLUDEDIR/libbfio/libbfio_libcpath.h \
$$INCLUDEDIR/libbfio/libbfio_libcthreads.h \
$$INCLUDEDIR/libbfio/libbfio_libuna.h \
$$INCLUDEDIR/libbfio/libbfio_memory_range.h \
$$INCLUDEDIR/libbfio/libbfio_memory_range_io_handle.h \
$$INCLUDEDIR/libbfio/libbfio_pool.h \
$$INCLUDEDIR/libbfio/libbfio_support.h \
$$INCLUDEDIR/libbfio/libbfio_system_string.h \
$$INCLUDEDIR/libbfio/libbfio_types.h \
$$INCLUDEDIR/libbfio/libbfio_unused.h

INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$INCLUDEDIR/libbfio
INCLUDEPATH += $$INCLUDEDIR/libcerror
INCLUDEPATH += $$INCLUDEDIR/libcdata
INCLUDEPATH += $$SRCINCLUDEDIR/libcfile
INCLUDEPATH += $$SRCINCLUDEDIR/libuna
INCLUDEPATH += $$SRCINCLUDEDIR/libcpath
INCLUDEPATH += $$SRCINCLUDEDIR/libclocale


LIBS += -L"$$LIBDIR/libcerror" -llibcerror
LIBS += -L"$$LIBDIR/libcdata" -llibcdata
LIBS += -L"$$LIBDIR/libcfile" -llibcfile
LIBS += -L"$$LIBDIR/libuna" -llibuna
LIBS += -L"$$LIBDIR/libcpath" -llibcpath
LIBS += -L"$$LIBDIR/libclocale" -llibclocale
