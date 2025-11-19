cmake_minimum_required(VERSION 3.16)

project(libfwps VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${SRCINCLUDEDIR}/libfwps/libfwps_codepage.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_debug.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_definitions.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_error.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_extern.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_format_class_identifier.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_libcdata.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_libcerror.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_libcnotify.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_libfguid.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_libuna.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_notify.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_property_identifier.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_record.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_set.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_store.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_support.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_types.h
    ${SRCINCLUDEDIR}/libfwps/libfwps_unused.h
)

set(SOURCESFILE
    ${SRCDIR}/libfwps_debug.c
    ${SRCDIR}/libfwps_error.c
    ${SRCDIR}/libfwps_format_class_identifier.c
    ${SRCDIR}/libfwps_notify.c
    ${SRCDIR}/libfwps_property_identifier.c
    ${SRCDIR}/libfwps_record.c
    ${SRCDIR}/libfwps_set.c
    ${SRCDIR}/libfwps_store.c
    ${SRCDIR}/libfwps_support.c
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
    ${OLECFINCLUDEDIR}/libcdata
    ${SRCINCLUDEDIR}/libcnotify
    ${SRCINCLUDEDIR}/libfguid
    ${SRCINCLUDEDIR}/libuna
)
include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_LOCAL_LIBFWPS
    HAVE_LOCAL_LIBCNOTIFY
    HAVE_LOCAL_LIBCDATA
    HAVE_LOCAL_LIBFGUID
    HAVE_LOCAL_LIBUNA
)
