CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib
DEFINES += SPDLOG_BUILD_SHARED SPDLOG_COMPILED_LIB
include(../common.pri)
CONFIG += c++11
msvc {
    QMAKE_CXXFLAGS += /utf-8
}
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/spdlog/async.h \
    include/spdlog/async_logger-inl.h \
    include/spdlog/async_logger.h \
    include/spdlog/common-inl.h \
    include/spdlog/common.h \
    include/spdlog/formatter.h \
    include/spdlog/fwd.h \
    include/spdlog/logger-inl.h \
    include/spdlog/logger.h \
    include/spdlog/mdc.h \
    include/spdlog/pattern_formatter-inl.h \
    include/spdlog/pattern_formatter.h \
    include/spdlog/spdlog-inl.h \
    include/spdlog/spdlog.h \
    include/spdlog/stopwatch.h \
    include/spdlog/tweakme.h \
    include/spdlog/version.h

SOURCES += \
    src/async.cpp \
    src/bundled_fmtlib_format.cpp \
    src/cfg.cpp \
    src/color_sinks.cpp \
    src/file_sinks.cpp \
    src/spdlog.cpp \
    src/stdout_sinks.cpp
INCLUDEPATH += $$PWD/include
