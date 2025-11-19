cmake_minimum_required(VERSION 3.16)
project(libolecf_suite LANGUAGES C CXX)

# 全局控制静态/动态库
option(BUILD_SHARED_LIBS "Build shared instead of static libraries" OFF)

# 添加所有子库
add_subdirectory(src/libcerror)
add_subdirectory(src/libcthreads)
add_subdirectory(src/libcdata)
add_subdirectory(src/libclocale)
add_subdirectory(src/libcnotify)
add_subdirectory(src/libcsplit)
add_subdirectory(src/libuna)
add_subdirectory(src/libcfile)
add_subdirectory(src/libcpath)
add_subdirectory(src/libbfio)
add_subdirectory(src/libfdatetime)
add_subdirectory(src/libfguid)
add_subdirectory(src/libfole)
add_subdirectory(src/libfvalue)
add_subdirectory(src/libfwps)
add_subdirectory(src/libolecf)
