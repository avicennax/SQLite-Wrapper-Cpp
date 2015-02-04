cmake_minimum_required(VERSION 3.0)
project(SQLite-Wrapper)

include_directories("${PROJECT_SOURCE_DIR}/includes")

set(SOURCES_DIR "Source Files")
set(BINARYNAME sqltest)
add_library(SQLite-Wrapper ${SOURCES_DIR}/sqlite3.c ${SOURCES_DIR}/SQLite-Wrapper.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")
add_executable(${BINARYNAME} ${SOURCES_DIR}/main.cpp)
target_link_libraries (${BINARYNAME} SQLite-Wrapper)