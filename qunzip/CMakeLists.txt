cmake_minimum_required(VERSION 3.16)

project(quazipself VERSION 0.1 LANGUAGES CXX)
include(../common.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core)

set(HEADFILE
    include/JlCompress.h
    include/ioapi.h
    include/minizip_crypt.h
    include/quaadler32.h
    include/quachecksum32.h
    include/quacrc32.h
    include/quagzipfile.h
    include/quaziodevice.h
    include/quazip.h
    include/quazip_global.h
    include/quazip_qt_compat.h
    include/quazipdir.h
    include/quazipfile.h
    include/quazipfileinfo.h
    include/quazipnewinfo.h
    include/unzip.h
    include/zip.h
)

set(SOURCESFILE
    src/JlCompress.cpp
    src/qioapi.cpp
    src/quaadler32.cpp
    src/quachecksum32.cpp
    src/quacrc32.cpp
    src/quagzipfile.cpp
    src/quaziodevice.cpp
    src/quazip.cpp
    src/quazipdir.cpp
    src/quazipfile.cpp
    src/quazipfileinfo.cpp
    src/quazipnewinfo.cpp
    src/unzip.c
    src/zip.c
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME} SHARED
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# include(GNUInstallDirs)
# install(TARGETS untitled
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )
