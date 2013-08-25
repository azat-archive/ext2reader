
cmake_minimum_required(VERSION 2.8)
project(Ext2Reader C)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
include(AddCompilerFlags)
include(FindLibrary)

AddCompilerFlags(FAIL_ON_ERROR FLAGS -g3 -O0 LANGUAGES C)
FindLibrary(FAIL_ON_ERROR NAMES ext2fs)
add_definitions(-D_GNU_SOURCE)

message(STATUS "Making ${PROJECT_NAME} on ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")

list(APPEND EXT2READER_SOURCES
     src/ext2reader.c
)

add_executable(ext2reader ${EXT2READER_SOURCES})
target_link_libraries(ext2reader ${LIBS})

