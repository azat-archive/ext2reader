
cmake_minimum_required(VERSION 2.8)
project(Ext2Reader C)

message(STATUS "Making ${PROJECT_NAME} on ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")

list(APPEND EXT2READER_SOURCES
     src/ext2reader.c
)

add_executable(ext2reader ${EXT2READER_SOURCES})
target_link_libraries(ext2reader ${LIBS})

