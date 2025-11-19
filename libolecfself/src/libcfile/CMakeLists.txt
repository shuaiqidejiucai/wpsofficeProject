cmake_minimum_required(VERSION 3.16)

project(libcfile VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${SRCINCLUDEDIR}/libcfile/libcfile_definitions.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_error.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_extern.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_file.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_libcerror.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_libclocale.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_libcnotify.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_libuna.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_notify.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_support.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_system_string.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_types.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_unused.h
    ${SRCINCLUDEDIR}/libcfile/libcfile_winapi.h
)

set(SOURCESFILE
    ${SRCDIR}/libcfile_error.c
    ${SRCDIR}/libcfile_file.c
    ${SRCDIR}/libcfile_notify.c
    ${SRCDIR}/libcfile_support.c
    ${SRCDIR}/libcfile_system_string.c
    ${SRCDIR}/libcfile_winapi.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRCINCLUDEDIR}/libcfile
    ${SRCINCLUDEDIR}/libclocale
    ${SRCINCLUDEDIR}/libuna
    ${SRCINCLUDEDIR}/libcnotify

)
include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_UNLINK
    HAVE_IOCTL
    HAVE_WRITE
    HAVE_READ
    HAVE_FSTAT
    HAVE_FCNTL_H
    HAVE_OPEN
    HAVE_SYS_STAT_H
    HAVE_STAT
    HAVE_LSEEK
    HAVE_ERRNO_H
    HAVE_CLOSE
    HAVE_LOCAL_LIBUNA
    HAVE_LOCAL_LIBCNOTIFY
    HAVE_LOCAL_LIBCLOCALE
    HAVE_LOCAL_LIBCFILE
)
