include(${CMAKE_CURRENT_LIST_DIR}/../../common.cmake)
set(SRCDIR ${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME})
set(SRCINCLUDEDIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(OLECFINCLUDEDIR ${CMAKE_CURRENT_LIST_DIR}/../include)

set(HEADFILE
    ${OLECFINCLUDEDIR}/common/byte_stream.h
    ${OLECFINCLUDEDIR}/common/config.h
    ${OLECFINCLUDEDIR}/common/file_stream.h
    ${OLECFINCLUDEDIR}/common/system_string.h
    ${OLECFINCLUDEDIR}/common/common.h
    ${OLECFINCLUDEDIR}/common/config_msc.h
    ${OLECFINCLUDEDIR}/common/memory.h
    ${OLECFINCLUDEDIR}/common/types.h
    ${OLECFINCLUDEDIR}/common/config_borlandc.h
    ${OLECFINCLUDEDIR}/common/config_winapi.h
    ${OLECFINCLUDEDIR}/common/narrow_string.h
    ${OLECFINCLUDEDIR}/common/wide_string.h
)



