cmake_minimum_required(VERSION 3.0)
project(SQLite-Wrapper)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
	add_definitions("-g3 -static-libgcc -std=c++0x")

	set(BINARYNAME sqltest)
	add_library(SQLite-Wrapper sources/sqlite3.c sources/SQLite-Wrapper.cpp)

	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")
	add_executable(${BINARYNAME} sources/main.cpp)
	target_link_libraries (${BINARYNAME} SQLite-Wrapper)
	
elseif (MSVC)
	include_directories("${PROJECT_SOURCE_DIR}/Headers")
	set(SOURCES_DIR "Source Files")
	set(BINARYNAME sqltest)
	add_library(SQLite-Wrapper ${SOURCES_DIR}/sqlite3.c ${SOURCES_DIR}/SQLite-Wrapper.cpp)
	add_library(SQLite-Wrapper SHARED ${SOURCES_DIR}/sqlite3.c ${SOURCES_DIR}/SQLite-Wrapper.cpp)

	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")
	add_executable(${BINARYNAME} ${SOURCES_DIR}/main.cpp)
	target_link_libraries (${BINARYNAME} SQLite-Wrapper)
	
endif ()