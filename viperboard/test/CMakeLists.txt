cmake_minimum_required(VERSION 3.2)

add_library(usb_mock STATIC libusb_mock.cpp)
add_dependencies(usb_mock googletest)
target_include_directories(usb_mock PUBLIC ${GTEST_INCLUDE})
target_link_libraries(usb_mock ${GTEST_LIBRARIES})

add_executable(test_viperboard test_viperboard_openclose.cpp test_viperboard_general.cpp test_viperboard_i2c_master.cpp)
target_link_libraries(test_viperboard viperboard usb_mock)
add_test(Viperboard test_viperboard)


if(CREATE_COVERAGE_TARGETS)
    SETUP_TARGET_FOR_COVERAGE(NAME test_viperboard_coverage
                              EXECUTABLE
                                  test_viperboard
                              DEPENDENCIES
                                  test_viperboard)

endif()
