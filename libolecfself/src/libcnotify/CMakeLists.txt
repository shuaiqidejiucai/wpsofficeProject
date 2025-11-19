cmake_minimum_required(VERSION 3.16)

project(libcnotify VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${SRCINCLUDEDIR}/libcnotify/libcnotify_definitions.h
    ${SRCINCLUDEDIR}/libcnotify/libcnotify_extern.h
    ${SRCINCLUDEDIR}/libcnotify/libcnotify_libcerror.h
    ${SRCINCLUDEDIR}/libcnotify/libcnotify_print.h
    ${SRCINCLUDEDIR}/libcnotify/libcnotify_stream.h
    ${SRCINCLUDEDIR}/libcnotify/libcnotify_support.h
    ${SRCINCLUDEDIR}/libcnotify/libcnotify_unused.h
    ${SRCINCLUDEDIR}/libcnotify/libcnotify_verbose.h
)

set(SOURCESFILE
    ${SRCDIR}/libcnotify_print.c
    ${SRCDIR}/libcnotify_stream.c
    ${SRCDIR}/libcnotify_support.c
    ${SRCDIR}/libcnotify_verbose.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRCINCLUDEDIR}/libcnotify
)
include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_ERRNO_H
    HAVE_STDARG_H
    HAVE_LOCAL_LIBCNOTIFY
)
