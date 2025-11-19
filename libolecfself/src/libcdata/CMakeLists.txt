cmake_minimum_required(VERSION 3.16)

project(libcdata VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${OLECFINCLUDEDIR}/libcdata/libcdata_array.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_btree.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_btree_node.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_btree_values_list.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_definitions.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_error.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_extern.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_libcerror.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_libcthreads.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_list.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_list_element.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_range_list.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_range_list_value.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_support.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_tree_node.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_types.h
    ${OLECFINCLUDEDIR}/libcdata/libcdata_unused.h
)

set(SOURCESFILE
    ${SRCDIR}/libcdata_array.c
    ${SRCDIR}/libcdata_btree.c
    ${SRCDIR}/libcdata_btree_node.c
    ${SRCDIR}/libcdata_btree_values_list.c
    ${SRCDIR}/libcdata_error.c
    ${SRCDIR}/libcdata_list.c
    ${SRCDIR}/libcdata_list_element.c
    ${SRCDIR}/libcdata_range_list.c
    ${SRCDIR}/libcdata_range_list_value.c
    ${SRCDIR}/libcdata_support.c
    ${SRCDIR}/libcdata_tree_node.c
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
    HAVE_LOCAL_LIBCDATA
)
