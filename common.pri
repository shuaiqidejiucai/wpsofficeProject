CONFIG(debug, debug|release){
platform=debug
}
CONFIG(release, debug|release){
platform=release
}

contains(QMAKE_CXX, clang++){
COMPILENAME=clang
}
contains(QMAKE_CXX, g++){
COMPILENAME=gcc
}

contains(QMAKE_CC, clang){
COMPILENAME=clang
}
contains(QMAKE_CC, gcc){
COMPILENAME=gcc
}
win32:msvc* {
    COMPILENAME=mscv
}
#release:DEFINES+=QT_NO_DEBUG_OUTPUT
OBJECTS_DIR = $$PWD/build/$$COMPILENAME/$$platform/$$TARGET/obj
MOC_DIR = $$PWD/build/$$COMPILENAME/$$platform/$$TARGET/moc
UI_DIR = $$PWD/build/$$COMPILENAME/$$platform/$$TARGET/ui
RCC_DIR = $$PWD/build/$$COMPILENAME/$$platform/$$TARGET/rcc
DESTDIR = $$PWD/lib/$$COMPILENAME/$$platform/$$TARGET

LIBDIR = $$PWD/lib/$$COMPILENAME/$$platform

INCLUDEDIR=$$PWD/include

equals(TEMPLATE, app){
DESTDIR = $$PWD/build/$$COMPILENAME/$$platform/$$TARGET

}
