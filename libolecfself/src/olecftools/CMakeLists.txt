cmake_minimum_required(VERSION 3.16)

project(olecftools VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${SRCINCLUDEDIR}/olecftools/export_handle.h
    ${SRCINCLUDEDIR}/olecftools/info_handle.h
    ${SRCINCLUDEDIR}/olecftools/log_handle.h
    ${SRCINCLUDEDIR}/olecftools/mount_dokan.h
    ${SRCINCLUDEDIR}/olecftools/mount_file_entry.h
    ${SRCINCLUDEDIR}/olecftools/mount_file_system.h
    ${SRCINCLUDEDIR}/olecftools/mount_fuse.h
    ${SRCINCLUDEDIR}/olecftools/mount_handle.h
    ${SRCINCLUDEDIR}/olecftools/mount_path_string.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_getopt.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_i18n.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_libbfio.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_libcerror.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_libcfile.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_libclocale.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_libcnotify.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_libcpath.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_libfdatetime.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_libfguid.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_libfole.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_libolecf.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_libuna.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_output.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_signal.h
    ${SRCINCLUDEDIR}/olecftools/olecftools_unused.h
)

set(SOURCESFILE
    ${SRCDIR}/export_handle.c
    ${SRCDIR}/info_handle.c
    ${SRCDIR}/log_handle.c
    ${SRCDIR}/mount_dokan.c
    ${SRCDIR}/mount_file_entry.c
    ${SRCDIR}/mount_file_system.c
    ${SRCDIR}/mount_fuse.c
    ${SRCDIR}/mount_handle.c
    ${SRCDIR}/mount_path_string.c
    ${SRCDIR}/olecfexport.c
    ${SRCDIR}/olecfinfo.c
    ${SRCDIR}/olecfmount.c
    ${SRCDIR}/olecftools_getopt.c
    ${SRCDIR}/olecftools_output.c
    ${SRCDIR}/olecftools_signal.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME} SHARED
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRCINCLUDEDIR}/olecftools
    ${OLECFINCLUDEDIR}/libbfio
    ${OLECFINCLUDEDIR}/libcdata
    ${SRCINCLUDEDIR}/libcnotify
    ${SRCINCLUDEDIR}/libfdatetime
    ${SRCINCLUDEDIR}/libfguid
    ${SRCINCLUDEDIR}/libfole
    ${SRCINCLUDEDIR}/libcfile
    ${SRCINCLUDEDIR}/libuna
    ${SRCINCLUDEDIR}/libcpath
    ${SRCINCLUDEDIR}/libclocale
)
include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_LOCAL_LIBCDATA
    HAVE_LOCAL_LIBFOLE
    HAVE_STDLIB_H
    HAVE_LOCAL_LIBFGUID
    HAVE_LOCAL_LIBBFIO
    HAVE_SIGNAL_H
    HAVE_LOCAL_LIBUNA
    HAVE_LOCAL_LIBFDATETIME
    HAVE_LOCAL_LIBCPATH
    HAVE_LOCAL_LIBCNOTIFY
    HAVE_LOCAL_LIBCLOCALE
    HAVE_LOCAL_LIBCFILE
    HAVE_STDARG_H
    $<$<PLATFORM_ID:Linux>:HAVE_UNISTD_H>
)
