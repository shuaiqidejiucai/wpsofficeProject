cmake_minimum_required(VERSION 3.16)

project(libcerror VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${OLECFINCLUDEDIR}/libcerror/libcerror_definitions.h
    ${OLECFINCLUDEDIR}/libcerror/libcerror_error.h
    ${OLECFINCLUDEDIR}/libcerror/libcerror_extern.h
    ${OLECFINCLUDEDIR}/libcerror/libcerror_support.h
    ${OLECFINCLUDEDIR}/libcerror/libcerror_system.h
    ${OLECFINCLUDEDIR}/libcerror/libcerror_types.h
    ${OLECFINCLUDEDIR}/libcerror/libcerror_unused.h
)

set(SOURCESFILE
    ${SRCDIR}/libcerror_error.c
    ${SRCDIR}/libcerror_support.c
    ${SRCDIR}/libcerror_system.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)


include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_STRERROR
    HAVE_STDARG_H
)
