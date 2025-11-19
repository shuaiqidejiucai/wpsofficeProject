cmake_minimum_required(VERSION 3.16)

project(libbfio VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${OLECFINCLUDEDIR}/libbfio/libbfio_codepage.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_definitions.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_error.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_extern.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_file.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_file_io_handle.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_file_pool.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_file_range.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_file_range_io_handle.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_handle.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_libcdata.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_libcerror.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_libcfile.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_libclocale.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_libcpath.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_libcthreads.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_libuna.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_memory_range.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_memory_range_io_handle.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_pool.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_support.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_system_string.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_types.h
    ${OLECFINCLUDEDIR}/libbfio/libbfio_unused.h

)

set(SOURCESFILE
    ${SRCDIR}/libbfio_error.c
    ${SRCDIR}/libbfio_file.c
    ${SRCDIR}/libbfio_file_io_handle.c
    ${SRCDIR}/libbfio_file_pool.c
    ${SRCDIR}/libbfio_file_range.c
    ${SRCDIR}/libbfio_file_range_io_handle.c
    ${SRCDIR}/libbfio_handle.c
    ${SRCDIR}/libbfio_memory_range.c
    ${SRCDIR}/libbfio_memory_range_io_handle.c
    ${SRCDIR}/libbfio_pool.c
    ${SRCDIR}/libbfio_support.c
    ${SRCDIR}/libbfio_system_string.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${OLECFINCLUDEDIR}/libbfio
    ${OLECFINCLUDEDIR}/libcerror
    ${OLECFINCLUDEDIR}/libcdata
    ${SRCINCLUDEDIR}/libcfile
    ${SRCINCLUDEDIR}/libuna
    ${SRCINCLUDEDIR}/libcpath
    ${SRCINCLUDEDIR}/libclocale
)

include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_LOCAL_LIBCPATH
    HAVE_LOCAL_LIBUNA
    HAVE_LOCAL_LIBCDATA
    HAVE_LOCAL_LIBCLOCALE
    HAVE_LOCAL_LIBCFILE
    HAVE_LOCAL_LIBBFIO
)
