cmake_minimum_required(VERSION 3.16)

project(libcpath VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${SRCINCLUDEDIR}/libcpath/libcpath_definitions.h
    ${SRCINCLUDEDIR}/libcpath/libcpath_error.h
    ${SRCINCLUDEDIR}/libcpath/libcpath_extern.h
    ${SRCINCLUDEDIR}/libcpath/libcpath_libcerror.h
    ${SRCINCLUDEDIR}/libcpath/libcpath_libclocale.h
    ${SRCINCLUDEDIR}/libcpath/libcpath_libcsplit.h
    ${SRCINCLUDEDIR}/libcpath/libcpath_libuna.h
    ${SRCINCLUDEDIR}/libcpath/libcpath_path.h
    ${SRCINCLUDEDIR}/libcpath/libcpath_support.h
    ${SRCINCLUDEDIR}/libcpath/libcpath_system_string.h
    ${SRCINCLUDEDIR}/libcpath/libcpath_unused.h

)

set(SOURCESFILE
    ${SRCDIR}/libcpath_error.c
    ${SRCDIR}/libcpath_path.c
    ${SRCDIR}/libcpath_support.c
    ${SRCDIR}/libcpath_system_string.c
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
    ${SRCINCLUDEDIR}/libcpath
    ${SRCINCLUDEDIR}/libclocale
    ${SRCINCLUDEDIR}/libuna
    ${SRCINCLUDEDIR}/libcsplit
)
include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_UNISTD_H
    HAVE_SYS_STAT_H
    HAVE_ERRNO_H
    HAVE_LIMITS_H
    HAVE_GETCWD
    HAVE_LOCAL_LIBUNA
    HAVE_LOCAL_LIBCSPLIT
    HAVE_LOCAL_LIBCLOCALE
    HAVE_LOCAL_LIBCPATH
)
