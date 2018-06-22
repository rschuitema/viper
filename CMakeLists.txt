cmake_minimum_required(VERSION 3.2)

project(viperboard)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_definitions(-std=c++11)

include(googletest)

add_subdirectory(viperboard)
