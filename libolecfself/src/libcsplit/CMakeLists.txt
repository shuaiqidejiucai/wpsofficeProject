cmake_minimum_required(VERSION 3.16)

project(libcsplit VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${SRCINCLUDEDIR}/libcsplit/libcsplit_definitions.h
    ${SRCINCLUDEDIR}/libcsplit/libcsplit_error.h
    ${SRCINCLUDEDIR}/libcsplit/libcsplit_extern.h
    ${SRCINCLUDEDIR}/libcsplit/libcsplit_libcerror.h
    ${SRCINCLUDEDIR}/libcsplit/libcsplit_narrow_split_string.h
    ${SRCINCLUDEDIR}/libcsplit/libcsplit_narrow_string.h
    ${SRCINCLUDEDIR}/libcsplit/libcsplit_support.h
    ${SRCINCLUDEDIR}/libcsplit/libcsplit_types.h
    ${SRCINCLUDEDIR}/libcsplit/libcsplit_unused.h
    ${SRCINCLUDEDIR}/libcsplit/libcsplit_wide_split_string.h
    ${SRCINCLUDEDIR}/libcsplit/libcsplit_wide_string.h
)

set(SOURCESFILE
    ${SRCDIR}/libcsplit_error.c
    ${SRCDIR}/libcsplit_narrow_split_string.c
    ${SRCDIR}/libcsplit_narrow_string.c
    ${SRCDIR}/libcsplit_support.c
    ${SRCDIR}/libcsplit_wide_split_string.c
    ${SRCDIR}/libcsplit_wide_string.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRCINCLUDEDIR}/libcsplit
)
include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_LOCAL_LIBCSPLIT
)
