cmake_minimum_required(VERSION 3.16)

project(libfguid VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${SRCINCLUDEDIR}/libfguid/libfguid_definitions.h
    ${SRCINCLUDEDIR}/libfguid/libfguid_error.h
    ${SRCINCLUDEDIR}/libfguid/libfguid_extern.h
    ${SRCINCLUDEDIR}/libfguid/libfguid_identifier.h
    ${SRCINCLUDEDIR}/libfguid/libfguid_libcerror.h
    ${SRCINCLUDEDIR}/libfguid/libfguid_support.h
    ${SRCINCLUDEDIR}/libfguid/libfguid_types.h
    ${SRCINCLUDEDIR}/libfguid/libfguid_unused.h
)

set(SOURCESFILE
    ${SRCDIR}/libfguid_error.c
    ${SRCDIR}/libfguid_identifier.c
    ${SRCDIR}/libfguid_support.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRCINCLUDEDIR}/libfguid
)
include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_LOCAL_LIBFGUID
)
