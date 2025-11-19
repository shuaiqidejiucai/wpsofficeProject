cmake_minimum_required(VERSION 3.16)

project(sqdlog VERSION 0.1 LANGUAGES C CXX)
include(../common.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADFILE
    include/spdlog/async.h
    include/spdlog/async_logger-inl.h
    include/spdlog/async_logger.h
    include/spdlog/common-inl.h
    include/spdlog/common.h
    include/spdlog/formatter.h
    include/spdlog/fwd.h
    include/spdlog/logger-inl.h
    include/spdlog/logger.h
    include/spdlog/mdc.h
    include/spdlog/pattern_formatter-inl.h
    include/spdlog/pattern_formatter.h
    include/spdlog/spdlog-inl.h
    include/spdlog/spdlog.h
    include/spdlog/stopwatch.h
    include/spdlog/tweakme.h
    include/spdlog/version.h
)

set(SOURCESFILE
    src/async.cpp
    src/bundled_fmtlib_format.cpp
    src/cfg.cpp
    src/color_sinks.cpp
    src/file_sinks.cpp
    src/spdlog.cpp
    src/stdout_sinks.cpp
)

set(PROJECT_SOURCES
   ${HEADFILE}
   ${SOURCESFILE}
)

add_library(${PROJECT_NAME} STATIC
        ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    SPDLOG_BUILD_SHARED
    SPDLOG_COMPILED_LIB
)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()
# install(TARGETS untitled
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )
