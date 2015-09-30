
# Project configuration.
cmake_minimum_required(VERSION 3.0)
project(lingo CXX)
enable_testing()


# Compiler configuration
set(CMAKE_CXX_FLAGS "-Wall -std=c++1y")


# Boost dependencies
find_package(Boost 1.55.0 REQUIRED
  COMPONENTS system filesystem)


# Build configuration.
include_directories(. ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})


add_executable(calc 
  main.cpp
  string.cpp
  symbol.cpp
  token.cpp
  ast.cpp
  lexer.cpp
  parser.cpp)