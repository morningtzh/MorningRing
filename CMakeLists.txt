# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for CMake to work correctly
cmake_minimum_required(VERSION 3.16)

set(MORNINGRING_VERSION "\"Hello World From CMake!\"")
set(MORNINGRING_BUILD_ID "\"Hello World From CMake!\"")

execute_process(COMMAND  "date" "+%s" OUTPUT_VARIABLE timestamp OUTPUT_STRIP_TRAILING_WHITESPACE)

MESSAGE(STATUS "Start Config ${timestamp} ${CMAKE_CURRENT_SOURCE_DIR}" )
set(EDIT_WARNING "/**** 本头文件为CMake生成文件，请修改相应 .in 文件 ****/")

set(VERSION_MAJOR 1 CACHE INTERNAL VersionMajor)
set(VERSION_MINOR 1 CACHE INTERNAL VersionMinor)
set(VERSION_PATCH 0 CACHE INTERNAL VersionPatch)
set(VERSION_BUILD_TIME ${timestamp} CACHE INTERNAL VersionBuildTime)

configure_file(main/utils/global.hpp.in ../main/utils/global.hpp)
MESSAGE("End Config")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(MorningRing)
