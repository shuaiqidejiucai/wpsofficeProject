CONFIG -= qt

TEMPLATE = lib

CONFIG += c++11
include(../common.pri)
DEFINES += LOG4CPP_HAVE_SSTREAM
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/AbortAppender.cpp \
    src/Appender.cpp \
    src/AppenderSkeleton.cpp \
    src/AppendersFactory.cpp \
    src/BasicConfigurator.cpp \
    src/BasicLayout.cpp \
    src/BufferingAppender.cpp \
    src/Category.cpp \
    src/CategoryStream.cpp \
    src/Configurator.cpp \
    src/DailyRollingFileAppender.cpp \
    src/DllMain.cpp \
    src/DummyThreads.cpp \
    src/FactoryParams.cpp \
    src/FileAppender.cpp \
    src/Filter.cpp \
    src/FixedContextCategory.cpp \
    src/HierarchyMaintainer.cpp \
    src/IdsaAppender.cpp \
    src/LayoutAppender.cpp \
    src/LayoutsFactory.cpp \
    src/LevelEvaluator.cpp \
    src/Localtime.cpp \
    src/LoggingEvent.cpp \
    src/MSThreads.cpp \
    src/Manipulator.cpp \
    src/NDC.cpp \
    src/NTEventLogAppender.cpp \
    src/OmniThreads.cpp \
    src/OstreamAppender.cpp \
    src/PThreads.cpp \
    src/PassThroughLayout.cpp \
    src/PatternLayout.cpp \
    src/PortabilityImpl.cpp \
    src/Priority.cpp \
    src/Properties.cpp \
    src/PropertyConfigurator.cpp \
    src/PropertyConfiguratorImpl.cpp \
    src/RemoteSyslogAppender.cpp \
    src/RollingFileAppender.cpp \
    src/SimpleConfigurator.cpp \
    src/SimpleLayout.cpp \
    src/SmtpAppender.cpp \
    src/StringQueueAppender.cpp \
    src/StringUtil.cpp \
    src/SyslogAppender.cpp \
    src/TimeStamp.cpp \
    src/TriggeringEventEvaluatorFactory.cpp \
    src/Win32DebugAppender.cpp \
    src/snprintf.c

HEADERS += \
    include/log4cpp/AbortAppender.hh \
    include/log4cpp/Appender.hh \
    include/log4cpp/AppenderSkeleton.hh \
    include/log4cpp/AppendersFactory.hh \
    include/log4cpp/BasicConfigurator.hh \
    include/log4cpp/BasicLayout.hh \
    include/log4cpp/BufferingAppender.hh \
    include/log4cpp/Category.hh \
    include/log4cpp/CategoryStream.hh \
    include/log4cpp/Configurator.hh \
    include/log4cpp/DailyRollingFileAppender.hh \
    include/log4cpp/Export.hh \
    include/log4cpp/FactoryParams.hh \
    include/log4cpp/FileAppender.hh \
    include/log4cpp/Filter.hh \
    include/log4cpp/FixedContextCategory.hh \
    include/log4cpp/HierarchyMaintainer.hh \
    include/log4cpp/IdsaAppender.hh \
    include/log4cpp/Layout.hh \
    include/log4cpp/LayoutAppender.hh \
    include/log4cpp/LayoutsFactory.hh \
    include/log4cpp/LevelEvaluator.hh \
    include/log4cpp/LoggingEvent.hh \
    include/log4cpp/Manipulator.hh \
    include/log4cpp/NDC.hh \
    include/log4cpp/NTEventLogAppender.hh \
    include/log4cpp/OstreamAppender.hh \
    include/log4cpp/PassThroughLayout.hh \
    include/log4cpp/PatternLayout.hh \
    include/log4cpp/Portability.hh \
    include/log4cpp/Priority.hh \
    include/log4cpp/PropertyConfigurator.hh \
    include/log4cpp/RemoteSyslogAppender.hh \
    include/log4cpp/RollingFileAppender.hh \
    include/log4cpp/SimpleConfigurator.hh \
    include/log4cpp/SimpleLayout.hh \
    include/log4cpp/SmtpAppender.hh \
    include/log4cpp/StringQueueAppender.hh \
    include/log4cpp/SyslogAppender.hh \
    include/log4cpp/TimeStamp.hh \
    include/log4cpp/TriggeringEventEvaluator.hh \
    include/log4cpp/TriggeringEventEvaluatorFactory.hh \
    include/log4cpp/Win32DebugAppender.hh \
    include/log4cpp/config-MinGW32.h \
    include/log4cpp/config-openvms.h \
    include/log4cpp/config-win32-stlport-boost.h \
    include/log4cpp/config-win32.h \
    include/log4cpp/convenience.h \
    include/log4cpp/threading/BoostThreads.hh \
    include/log4cpp/threading/DummyThreads.hh \
    include/log4cpp/threading/MSThreads.hh \
    include/log4cpp/threading/OmniThreads.hh \
    include/log4cpp/threading/PThreads.hh \
    include/log4cpp/threading/Threading.hh \
    src/Localtime.hh \
    src/PortabilityImpl.hh \
    src/Properties.hh \
    src/PropertyConfiguratorImpl.hh \
    src/StringUtil.hh

INCLUDEPATH += $$PWD/include

win32{
DEFINES += LOG4CPP_HAS_DLL LOG4CPP_BUILD_DLL
msvc{
DEFINES += LOG4CPP_HAVE_IO_H
}
}
unix:!macx{
DEFINES += LOG4CPP_HAVE_UNISTD_H
}
# Default rules for deployment.
