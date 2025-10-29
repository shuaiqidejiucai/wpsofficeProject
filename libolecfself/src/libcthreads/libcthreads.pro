include(../publicArgs.pri)

DEFINES += HAVE_LOCAL_LIBCTHREADS
HEADERS += \
$$SRCINCLUDEDIR/libcthreads/libcthreads_condition.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_definitions.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_error.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_extern.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_libcerror.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_lock.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_mutex.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_queue.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_read_write_lock.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_repeating_thread.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_support.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_thread.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_thread_attributes.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_thread_pool.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_types.h \
$$SRCINCLUDEDIR/libcthreads/libcthreads_unused.h

SOURCES += \
$$SRCDIR/libcthreads_condition.c \
$$SRCDIR/libcthreads_error.c \
$$SRCDIR/libcthreads_lock.c \
$$SRCDIR/libcthreads_mutex.c \
$$SRCDIR/libcthreads_queue.c \
$$SRCDIR/libcthreads_read_write_lock.c \
$$SRCDIR/libcthreads_repeating_thread.c \
$$SRCDIR/libcthreads_support.c \
$$SRCDIR/libcthreads_thread.c \
$$SRCDIR/libcthreads_thread_attributes.c \
$$SRCDIR/libcthreads_thread_pool.c

INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$INCLUDEDIR/libcerror
INCLUDEPATH += $$SRCINCLUDEDIR/libcthreads


LIBS += -L"$$LIBDIR/libcerror" -llibcerror
