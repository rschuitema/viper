cmake_minimum_required(VERSION 3.2)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(
    VIPERBOARD_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/i2c_master_viperboard.h
)

set(
    VIPERBOARD_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/i2c_master_viperboard.cpp
)

add_library(viperboard ${VIPERBOARD_SOURCE_FILES})

