cmake_minimum_required(VERSION 3.0.0)
project(isam VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(csv2bin csv2bin.cpp record.cpp)
add_executable(index-gen index-gen.cpp record.cpp)
add_executable(isam isam.cpp record.cpp lexer.cpp statement.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
