cmake_minimum_required(VERSION 3.2)

message(STATUS "Checking if all libraries viper depends on are available")
find_package(libusb REQUIRED)

include_directories(${LIBUSB_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(
    VIPERBOARD_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/viperboard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/i2c_master_viperboard.h
)

set(
    VIPERBOARD_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/viperboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/i2c_master_viperboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gpio_portb_viperboard.cpp
)

add_library(viperboard ${VIPERBOARD_SOURCE_FILES})
target_link_libraries(viperboard ${LIBUSB_LIBRARIES})

add_subdirectory(test)

