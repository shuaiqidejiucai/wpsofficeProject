cmake_minimum_required(VERSION 3.16)

project(libfole VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${SRCINCLUDEDIR}/libfole/libfole_definitions.h
    ${SRCINCLUDEDIR}/libfole/libfole_error.h
    ${SRCINCLUDEDIR}/libfole/libfole_extern.h
    ${SRCINCLUDEDIR}/libfole/libfole_libcerror.h
    ${SRCINCLUDEDIR}/libfole/libfole_support.h
    ${SRCINCLUDEDIR}/libfole/libfole_types.h
    ${SRCINCLUDEDIR}/libfole/libfole_unused.h
    ${SRCINCLUDEDIR}/libfole/libfole_value_type.h

)

set(SOURCESFILE
    ${SRCDIR}/libfole_error.c
    ${SRCDIR}/libfole_support.c
    ${SRCDIR}/libfole_value_type.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRCINCLUDEDIR}/libfole
)
include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_LOCAL_LIBFOLE
)
