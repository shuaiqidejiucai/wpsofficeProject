cmake_minimum_required(VERSION 3.16)


project(pyolecf VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PYTHON_PATH /usr/include/python3.12)
list(APPEND HEADFILE
    ${SRCINCLUDEDIR}/pyolecf/pyolecf.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_codepage.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_datetime.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_error.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_file.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_file_object_io_handle.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_guid.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_integer.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_item.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_item_types.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_items.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_libbfio.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_libcerror.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_libclocale.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_libfguid.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_libolecf.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_property_section.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_property_sections.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_property_set.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_property_set_stream.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_property_value.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_property_values.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_python.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_stream.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_unused.h
    ${SRCINCLUDEDIR}/pyolecf/pyolecf_value_types.h
)

set(SOURCESFILE
    ${SRCDIR}/pyolecf.c
    ${SRCDIR}/pyolecf_codepage.c
    ${SRCDIR}/pyolecf_datetime.c
    ${SRCDIR}/pyolecf_error.c
    ${SRCDIR}/pyolecf_file.c
    ${SRCDIR}/pyolecf_file_object_io_handle.c
    ${SRCDIR}/pyolecf_guid.c
    ${SRCDIR}/pyolecf_integer.c
    ${SRCDIR}/pyolecf_item.c
    ${SRCDIR}/pyolecf_item_types.c
    ${SRCDIR}/pyolecf_items.c
    ${SRCDIR}/pyolecf_property_section.c
    ${SRCDIR}/pyolecf_property_sections.c
    ${SRCDIR}/pyolecf_property_set.c
    ${SRCDIR}/pyolecf_property_set_stream.c
    ${SRCDIR}/pyolecf_property_value.c
    ${SRCDIR}/pyolecf_property_values.c
    ${SRCDIR}/pyolecf_stream.c
    ${SRCDIR}/pyolecf_value_types.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME} SHARED
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PYTHON_PATH}
    ${SRCINCLUDEDIR}/pyolecf
    ${OLECFINCLUDEDIR}/libbfio
    ${OLECFINCLUDEDIR}/libcdata
    ${SRCINCLUDEDIR}/libfguid
    ${SRCINCLUDEDIR}/libclocale
)
include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_LOCAL_LIBCLOCALE
    HAVE_LOCAL_LIBCDATA
    HAVE_LOCAL_LIBFGUID
    HAVE_STDARG_H
    HAVE_LOCAL_LIBBFIO
    $<$<PLATFORM_ID:Linux>:HAVE_UNISTD_H>
)
