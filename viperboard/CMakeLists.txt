cmake_minimum_required(VERSION 3.2)

message(STATUS "Checking if all libraries viper depends on are available")
find_package(libusb REQUIRED)

include_directories(${LIBUSB_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(
    VIPERBOARD_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/viperboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/i2c_master_viperboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gpio_portb_viperboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gpio_porta_viperboard.cpp
)

add_library(viperboard ${VIPERBOARD_SOURCE_FILES})
target_link_libraries(viperboard ${LIBUSB_LIBRARIES})



set(
    VIPERBOARD_TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_viperboard_openclose.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_viperboard_general.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_viperboard_gpio_porta.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_viperboard_gpio_portb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_viperboard_i2c_master.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_viperboard_i2c_master_write.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/libusb_mock.cpp
)

add_executable(test_viperboard ${VIPERBOARD_TEST_FILES})
target_link_libraries(test_viperboard viperboard gtest gmock gmock_main)

