cmake_minimum_required(VERSION 3.16)

project(libcthreads VERSION 0.1 LANGUAGES C CXX)
include(${CMAKE_CURRENT_LIST_DIR}/../publicargs.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND HEADFILE
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_condition.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_definitions.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_error.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_extern.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_libcerror.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_lock.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_mutex.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_queue.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_read_write_lock.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_repeating_thread.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_support.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_thread.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_thread_attributes.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_thread_pool.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_types.h
    ${SRCINCLUDEDIR}/libcthreads/libcthreads_unused.h

)

set(SOURCESFILE
    ${SRCDIR}/libcthreads_condition.c
    ${SRCDIR}/libcthreads_error.c
    ${SRCDIR}/libcthreads_lock.c
    ${SRCDIR}/libcthreads_mutex.c
    ${SRCDIR}/libcthreads_queue.c
    ${SRCDIR}/libcthreads_read_write_lock.c
    ${SRCDIR}/libcthreads_repeating_thread.c
    ${SRCDIR}/libcthreads_support.c
    ${SRCDIR}/libcthreads_thread.c
    ${SRCDIR}/libcthreads_thread_attributes.c
    ${SRCDIR}/libcthreads_thread_pool.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME}
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRCINCLUDEDIR}/libcthreads
)
include(../defines.cmake)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_LOCAL_LIBCTHREADS
)
