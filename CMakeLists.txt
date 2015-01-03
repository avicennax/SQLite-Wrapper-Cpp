cmake_minimum_required(VERSION 3.0)
project(SQLite-Wrapper)

add_definitions("-g3 -static-libgcc -std=c++0x")
include_directories("${PROJECT_SOURCE_DIR}/includes")

set(BINARYNAME sqltest)
add_library(SQLite-Wrapper sources/sqlite3.c sources/SQLite-Wrapper.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")
add_executable(${BINARYNAME} sources/main.cpp)
target_link_libraries (${BINARYNAME} SQLite-Wrapper)