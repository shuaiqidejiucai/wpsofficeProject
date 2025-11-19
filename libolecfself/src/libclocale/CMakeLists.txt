cmake_minimum_required(VERSION 3.16)

project(libclocale VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${SRCINCLUDEDIR}/libclocale/libclocale_codepage.h
    ${SRCINCLUDEDIR}/libclocale/libclocale_definitions.h
    ${SRCINCLUDEDIR}/libclocale/libclocale_extern.h
    ${SRCINCLUDEDIR}/libclocale/libclocale_libcerror.h
    ${SRCINCLUDEDIR}/libclocale/libclocale_locale.h
    ${SRCINCLUDEDIR}/libclocale/libclocale_support.h
    ${SRCINCLUDEDIR}/libclocale/libclocale_unused.h
    ${SRCINCLUDEDIR}/libclocale/libclocale_wide_string.h
)

set(SOURCESFILE
    ${SRCDIR}/libclocale_codepage.c
    ${SRCDIR}/libclocale_locale.c
    ${SRCDIR}/libclocale_support.c
    ${SRCDIR}/libclocale_wide_string.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRCINCLUDEDIR}/libclocale
)
include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_LOCALE_H
    HAVE_LOCAL_LIBCLOCALE
)
