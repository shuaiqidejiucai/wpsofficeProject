cmake_minimum_required(VERSION 3.16)

project(libolecf VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${OLECFINCLUDEDIR}/libolecf/libolecf.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf/codepage.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf/definitions.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf/error.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf/extern.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf/features.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf/ole.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf/types.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_allocation_table.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_codepage.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_compound_object_stream.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_debug.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_definitions.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_directory_entry.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_directory_list.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_directory_tree.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_error.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_extern.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_file.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_file_header.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_io_handle.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_item.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_libbfio.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_libcdata.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_libcerror.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_libclocale.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_libcnotify.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_libfdatetime.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_libfguid.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_libfole.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_libfvalue.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_libfwps.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_libuna.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_notify.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_ole.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_property_section.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_property_set.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_property_set_stream.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_property_value.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_stream.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_support.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_types.h
    ${OLECFINCLUDEDIR}/libolecf/libolecf_unused.h
    ${OLECFINCLUDEDIR}/libolecf/olecf_compound_object.h
    ${OLECFINCLUDEDIR}/libolecf/olecf_directory.h
    ${OLECFINCLUDEDIR}/libolecf/olecf_file_header.h
    ${OLECFINCLUDEDIR}/libolecf/olecf_property_set.h
)

set(SOURCESFILE
    ${SRCDIR}/libolecf.c
    ${SRCDIR}/libolecf_allocation_table.c
    ${SRCDIR}/libolecf_compound_object_stream.c
    ${SRCDIR}/libolecf_debug.c
    ${SRCDIR}/libolecf_directory_entry.c
    ${SRCDIR}/libolecf_directory_list.c
    ${SRCDIR}/libolecf_directory_tree.c
    ${SRCDIR}/libolecf_error.c
    ${SRCDIR}/libolecf_file.c
    ${SRCDIR}/libolecf_file_header.c
    ${SRCDIR}/libolecf_io_handle.c
    ${SRCDIR}/libolecf_item.c
    ${SRCDIR}/libolecf_notify.c
    ${SRCDIR}/libolecf_property_section.c
    ${SRCDIR}/libolecf_property_set.c
    ${SRCDIR}/libolecf_property_set_stream.c
    ${SRCDIR}/libolecf_property_value.c
    ${SRCDIR}/libolecf_stream.c
    ${SRCDIR}/libolecf_support.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRCINCLUDEDIR}/libfwps
    ${OLECFINCLUDEDIR}/libolecf
    ${OLECFINCLUDEDIR}/libbfio
    ${OLECFINCLUDEDIR}/libcdata
    ${SRCINCLUDEDIR}/libcnotify
    ${SRCINCLUDEDIR}/libfguid
    ${SRCINCLUDEDIR}/libuna
    ${SRCINCLUDEDIR}/libfdatetime
    ${SRCINCLUDEDIR}/libfole
    ${SRCINCLUDEDIR}/libfvalue
    ${SRCINCLUDEDIR}/libclocale
)
include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_LOCAL_LIBCLOCALE
    HAVE_LOCAL_LIBFVALUE
    HAVE_LOCAL_LIBFWPS
    HAVE_LOCAL_LIBFOLE
    HAVE_LOCAL_LIBFGUID
    HAVE_LOCAL_LIBUNA
    HAVE_LOCAL_LIBFDATETIME
    HAVE_LOCAL_LIBCDATA
    HAVE_LOCAL_LIBBFIO
    HAVE_LOCAL_LIBCNOTIFY
)
