
include(../publicArgs.pri)

DEFINES += HAVE_LOCAL_LIBCDATA

SOURCES += \
$$SRCDIR/libcdata_array.c \
$$SRCDIR/libcdata_btree.c \
$$SRCDIR/libcdata_btree_node.c \
$$SRCDIR/libcdata_btree_values_list.c \
$$SRCDIR/libcdata_error.c \
$$SRCDIR/libcdata_list.c \
$$SRCDIR/libcdata_list_element.c \
$$SRCDIR/libcdata_range_list.c \
$$SRCDIR/libcdata_range_list_value.c \
$$SRCDIR/libcdata_support.c \
$$SRCDIR/libcdata_tree_node.c

HEADERS += \
$$INCLUDEDIR/libcdata/libcdata_array.h \
$$INCLUDEDIR/libcdata/libcdata_btree.h \
$$INCLUDEDIR/libcdata/libcdata_btree_node.h \
$$INCLUDEDIR/libcdata/libcdata_btree_values_list.h \
$$INCLUDEDIR/libcdata/libcdata_definitions.h \
$$INCLUDEDIR/libcdata/libcdata_error.h \
$$INCLUDEDIR/libcdata/libcdata_extern.h \
$$INCLUDEDIR/libcdata/libcdata_libcerror.h \
$$INCLUDEDIR/libcdata/libcdata_libcthreads.h \
$$INCLUDEDIR/libcdata/libcdata_list.h \
$$INCLUDEDIR/libcdata/libcdata_list_element.h \
$$INCLUDEDIR/libcdata/libcdata_range_list.h \
$$INCLUDEDIR/libcdata/libcdata_range_list_value.h \
$$INCLUDEDIR/libcdata/libcdata_support.h \
$$INCLUDEDIR/libcdata/libcdata_tree_node.h \
$$INCLUDEDIR/libcdata/libcdata_types.h \
$$INCLUDEDIR/libcdata/libcdata_unused.h

INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$INCLUDEDIR/libcdata
INCLUDEPATH += $$INCLUDEDIR/libcerror

LIBS += -L"$$LIBDIR/libcerror" -llibcerror

