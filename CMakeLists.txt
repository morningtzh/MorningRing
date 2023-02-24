# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for CMake to work correctly
cmake_minimum_required(VERSION 3.16)

execute_process(COMMAND  "date" "+%s" OUTPUT_VARIABLE timestamp OUTPUT_STRIP_TRAILING_WHITESPACE)

MESSAGE(STATUS "Start Config ${timestamp} ${CMAKE_CURRENT_SOURCE_DIR}" )
set(EDIT_WARNING "/**** 本头文件为CMake生成文件，请修改相应 .in 文件 ****/")

set(VERSION_MAJOR 1 CACHE INTERNAL VersionMajor)
set(VERSION_MINOR 1 CACHE INTERNAL VersionMinor)
set(VERSION_PATCH 0 CACHE INTERNAL VersionPatch)
set(VERSION_BUILD_TIME ${timestamp} CACHE INTERNAL VersionBuildTime)

set(PROJECT_VER "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_BUILD_TIME}")
execute_process(COMMAND  "echo" "v${PROJECT_VER}" OUTPUT_FILE "version.txt" OUTPUT_STRIP_TRAILING_WHITESPACE)

configure_file(main/utils/global.hpp.in ../main/utils/global.hpp)
MESSAGE("End Config")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(MorningRing)
