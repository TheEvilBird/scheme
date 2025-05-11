cmake_minimum_required(VERSION 3.13)

project(scheme)
option(ENABLE_SEMINARS OFF)

set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

include(cmake/BuildFlags.cmake)
include(cmake/TestSolution.cmake)

include(FetchContent)

find_package(Catch REQUIRED)

set(SCHEME_COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)
add_subdirectory(advanced)
