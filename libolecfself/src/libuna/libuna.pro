include(../publicArgs.pri)
DEFINES += HAVE_LOCAL_LIBUNA

SOURCES += \
$$SRCDIR/libuna_base16_stream.c \
$$SRCDIR/libuna_base32_stream.c \
$$SRCDIR/libuna_base64_stream.c \
$$SRCDIR/libuna_byte_stream.c \
$$SRCDIR/libuna_codepage_iso_8859_10.c \
$$SRCDIR/libuna_codepage_iso_8859_13.c \
$$SRCDIR/libuna_codepage_iso_8859_14.c \
$$SRCDIR/libuna_codepage_iso_8859_15.c \
$$SRCDIR/libuna_codepage_iso_8859_16.c \
$$SRCDIR/libuna_codepage_iso_8859_2.c \
$$SRCDIR/libuna_codepage_iso_8859_3.c \
$$SRCDIR/libuna_codepage_iso_8859_4.c \
$$SRCDIR/libuna_codepage_iso_8859_5.c \
$$SRCDIR/libuna_codepage_iso_8859_6.c \
$$SRCDIR/libuna_codepage_iso_8859_7.c \
$$SRCDIR/libuna_codepage_iso_8859_8.c \
$$SRCDIR/libuna_codepage_iso_8859_9.c \
$$SRCDIR/libuna_codepage_koi8_r.c \
$$SRCDIR/libuna_codepage_koi8_u.c \
$$SRCDIR/libuna_codepage_mac_arabic.c \
$$SRCDIR/libuna_codepage_mac_celtic.c \
$$SRCDIR/libuna_codepage_mac_centraleurroman.c \
$$SRCDIR/libuna_codepage_mac_croatian.c \
$$SRCDIR/libuna_codepage_mac_cyrillic.c \
$$SRCDIR/libuna_codepage_mac_dingbats.c \
$$SRCDIR/libuna_codepage_mac_farsi.c \
$$SRCDIR/libuna_codepage_mac_gaelic.c \
$$SRCDIR/libuna_codepage_mac_greek.c \
$$SRCDIR/libuna_codepage_mac_icelandic.c \
$$SRCDIR/libuna_codepage_mac_inuit.c \
$$SRCDIR/libuna_codepage_mac_roman.c \
$$SRCDIR/libuna_codepage_mac_romanian.c \
$$SRCDIR/libuna_codepage_mac_russian.c \
$$SRCDIR/libuna_codepage_mac_symbol.c \
$$SRCDIR/libuna_codepage_mac_thai.c \
$$SRCDIR/libuna_codepage_mac_turkish.c \
$$SRCDIR/libuna_codepage_mac_ukrainian.c \
$$SRCDIR/libuna_codepage_windows_1250.c \
$$SRCDIR/libuna_codepage_windows_1251.c \
$$SRCDIR/libuna_codepage_windows_1252.c \
$$SRCDIR/libuna_codepage_windows_1253.c \
$$SRCDIR/libuna_codepage_windows_1254.c \
$$SRCDIR/libuna_codepage_windows_1255.c \
$$SRCDIR/libuna_codepage_windows_1256.c \
$$SRCDIR/libuna_codepage_windows_1257.c \
$$SRCDIR/libuna_codepage_windows_1258.c \
$$SRCDIR/libuna_codepage_windows_874.c \
$$SRCDIR/libuna_codepage_windows_932.c \
$$SRCDIR/libuna_codepage_windows_936.c \
$$SRCDIR/libuna_codepage_windows_949.c \
$$SRCDIR/libuna_codepage_windows_950.c \
$$SRCDIR/libuna_error.c \
$$SRCDIR/libuna_scsu.c \
$$SRCDIR/libuna_support.c \
$$SRCDIR/libuna_unicode_character.c \
$$SRCDIR/libuna_url_stream.c \
$$SRCDIR/libuna_utf16_stream.c \
$$SRCDIR/libuna_utf16_string.c \
$$SRCDIR/libuna_utf32_stream.c \
$$SRCDIR/libuna_utf32_string.c \
$$SRCDIR/libuna_utf7_stream.c \
$$SRCDIR/libuna_utf8_stream.c \
$$SRCDIR/libuna_utf8_string.c

HEADERS += \
$$SRCINCLUDEDIR/libuna/libuna_base16_stream.h \
$$SRCINCLUDEDIR/libuna/libuna_base32_stream.h \
$$SRCINCLUDEDIR/libuna/libuna_base64_stream.h \
$$SRCINCLUDEDIR/libuna/libuna_byte_stream.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_10.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_13.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_14.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_15.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_16.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_2.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_3.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_4.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_5.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_6.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_7.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_8.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_iso_8859_9.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_koi8_r.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_koi8_u.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_arabic.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_celtic.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_centraleurroman.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_croatian.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_cyrillic.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_dingbats.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_farsi.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_gaelic.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_greek.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_icelandic.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_inuit.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_roman.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_romanian.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_russian.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_symbol.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_thai.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_turkish.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_mac_ukrainian.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_1250.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_1251.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_1252.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_1253.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_1254.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_1255.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_1256.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_1257.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_1258.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_874.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_932.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_936.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_949.h \
$$SRCINCLUDEDIR/libuna/libuna_codepage_windows_950.h \
$$SRCINCLUDEDIR/libuna/libuna_definitions.h \
$$SRCINCLUDEDIR/libuna/libuna_error.h \
$$SRCINCLUDEDIR/libuna/libuna_extern.h \
$$SRCINCLUDEDIR/libuna/libuna_libcerror.h \
$$SRCINCLUDEDIR/libuna/libuna_scsu.h \
$$SRCINCLUDEDIR/libuna/libuna_support.h \
$$SRCINCLUDEDIR/libuna/libuna_types.h \
$$SRCINCLUDEDIR/libuna/libuna_unicode_character.h \
$$SRCINCLUDEDIR/libuna/libuna_unused.h \
$$SRCINCLUDEDIR/libuna/libuna_url_stream.h \
$$SRCINCLUDEDIR/libuna/libuna_utf16_stream.h \
$$SRCINCLUDEDIR/libuna/libuna_utf16_string.h \
$$SRCINCLUDEDIR/libuna/libuna_utf32_stream.h \
$$SRCINCLUDEDIR/libuna/libuna_utf32_string.h \
$$SRCINCLUDEDIR/libuna/libuna_utf7_stream.h \
$$SRCINCLUDEDIR/libuna/libuna_utf8_stream.h \
$$SRCINCLUDEDIR/libuna/libuna_utf8_string.h

INCLUDEPATH += $$INCLUDEDIR/common
INCLUDEPATH += $$SRCINCLUDEDIR/libuna
INCLUDEPATH += $$INCLUDEDIR/libcerror

LIBS += -L"$$LIBDIR/libcerror" -llibcerror
