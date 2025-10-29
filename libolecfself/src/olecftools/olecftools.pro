
include(publicArgs.pri)

linux {
   DEFINES += HAVE_UNISTD_H
}
DEFINES += HAVE_LOCAL_LIBCDATA HAVE_LOCAL_LIBFOLE HAVE_STDLIB_H HAVE_LOCAL_LIBFGUID HAVE_LOCAL_LIBBFIO HAVE_SIGNAL_H HAVE_LOCAL_LIBUNA HAVE_LOCAL_LIBFDATETIME HAVE_LOCAL_LIBCPATH HAVE_LOCAL_LIBCNOTIFY HAVE_LOCAL_LIBCLOCALE HAVE_LOCAL_LIBCFILE HAVE_STDARG_H
HEADERS += \
$$SRCINCLUDEDIR/olecftools/export_handle.h \
$$SRCINCLUDEDIR/olecftools/info_handle.h \
$$SRCINCLUDEDIR/olecftools/log_handle.h \
$$SRCINCLUDEDIR/olecftools/mount_dokan.h \
$$SRCINCLUDEDIR/olecftools/mount_file_entry.h \
$$SRCINCLUDEDIR/olecftools/mount_file_system.h \
$$SRCINCLUDEDIR/olecftools/mount_fuse.h \
$$SRCINCLUDEDIR/olecftools/mount_handle.h \
$$SRCINCLUDEDIR/olecftools/mount_path_string.h \
$$SRCINCLUDEDIR/olecftools/olecftools_getopt.h \
$$SRCINCLUDEDIR/olecftools/olecftools_i18n.h \
$$SRCINCLUDEDIR/olecftools/olecftools_libbfio.h \
$$SRCINCLUDEDIR/olecftools/olecftools_libcerror.h \
$$SRCINCLUDEDIR/olecftools/olecftools_libcfile.h \
$$SRCINCLUDEDIR/olecftools/olecftools_libclocale.h \
$$SRCINCLUDEDIR/olecftools/olecftools_libcnotify.h \
$$SRCINCLUDEDIR/olecftools/olecftools_libcpath.h \
$$SRCINCLUDEDIR/olecftools/olecftools_libfdatetime.h \
$$SRCINCLUDEDIR/olecftools/olecftools_libfguid.h \
$$SRCINCLUDEDIR/olecftools/olecftools_libfole.h \
$$SRCINCLUDEDIR/olecftools/olecftools_libolecf.h \
$$SRCINCLUDEDIR/olecftools/olecftools_libuna.h \
$$SRCINCLUDEDIR/olecftools/olecftools_output.h \
$$SRCINCLUDEDIR/olecftools/olecftools_signal.h \
$$SRCINCLUDEDIR/olecftools/olecftools_unused.h

SOURCES += \
$$SRCDIR/export_handle.c \
$$SRCDIR/info_handle.c \
$$SRCDIR/log_handle.c \
$$SRCDIR/mount_dokan.c \
$$SRCDIR/mount_file_entry.c \
$$SRCDIR/mount_file_system.c \
$$SRCDIR/mount_fuse.c \
$$SRCDIR/mount_handle.c \
$$SRCDIR/mount_path_string.c \
$$SRCDIR/olecfexport.c \
$$SRCDIR/olecfinfo.c \
$$SRCDIR/olecfmount.c \
$$SRCDIR/olecftools_getopt.c \
$$SRCDIR/olecftools_output.c \
$$SRCDIR/olecftools_signal.c


INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$SRCINCLUDEDIR/olecftools
# INCLUDEPATH += $$PYTHON_PATH
# INCLUDEPATH += $$INCLUDEDIR/libolecf
# INCLUDEPATH += $$SRCINCLUDEDIR/libfwps
INCLUDEPATH += $$INCLUDEDIR/libbfio

INCLUDEPATH += $$INCLUDEDIR/libcdata
INCLUDEPATH += $$SRCINCLUDEDIR/libcnotify
INCLUDEPATH += $$SRCINCLUDEDIR/libfdatetime
INCLUDEPATH += $$SRCINCLUDEDIR/libfguid
INCLUDEPATH += $$SRCINCLUDEDIR/libfole
# INCLUDEPATH += $$SRCINCLUDEDIR/libfvalue
INCLUDEPATH += $$SRCINCLUDEDIR/libcfile
INCLUDEPATH += $$SRCINCLUDEDIR/libuna
INCLUDEPATH += $$SRCINCLUDEDIR/libcpath
INCLUDEPATH += $$SRCINCLUDEDIR/libclocale
