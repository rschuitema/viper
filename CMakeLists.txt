cmake_minimum_required(VERSION 3.2)

project(viperboard)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

message(STATUS CMAKE_BUILD_TYPE)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "build type is Debug")
    option(CREATE_COVERAGE_TARGETS "Create targets to run unit tests with coverage" OFF)
    if(CREATE_COVERAGE_TARGETS)
        message(STATUS "Adding targets for measuring code coverage")
        include(CodeCoverage)
        append_coverage_compiler_flags()
        set(COVERAGE_EXCLUDES 'boost/*'
                              'c++/*'
                              'x86_64-linux-gnu/*'
                              '*/gmock/*'
                              '*/simple_*'
                              '*/gtest/*')

        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/run_all_unit_tests.sh
                       ${CMAKE_CURRENT_BINARY_DIR}/run_all_unit_tests.sh COPYONLY)
        SETUP_TARGET_FOR_COVERAGE(NAME complete_coverage
                                  EXECUTABLE
                                      ${CMAKE_CURRENT_BINARY_DIR}/run_all_unit_tests.sh
                                  DEPENDENCIES
                                      test_portable_cypress_device
                                      test_portable_cypress_control_endpoint
                                      test_portable_cypress_bulk_endpoint
                                      test_libusb_device
                                      test_libusb_endpoint
                                      test_libusb_interface
                                      test_libusb_device_watcher
                                      test_utilities
                                      )
    endif()
endif()


## section: add definitions
#   add prefix -D. example> -DSHP
#  - DO NOT add  the following definitions(already defined in ${OSP_DEFINITIONS}:
#   -DSHP, -DWIN32, -D_WINDOWS, -D_DEBUG, -D_USRDLL, -D_CRT_SECURE_NO_DEPRECATE
add_definitions(-std=c++11)

# Set compiler flags as strict as possible: https://stackoverflow.com/a/45046157/1149326
set(CMAKE_CXX_FLAGS "-Wall \
                     -Wextra  \
                     -Wstrict-aliasing \
                     -pedantic \
                     -fmax-errors=5 \
                     -Werror \
                     -Wunreachable-code \
                     -Wcast-align \
                     -Wcast-qual \
                     -Wdisabled-optimization \
                     -Wformat=2 \
                     -Winit-self \
                     -Wlogical-op \
                     -Wmissing-include-dirs \
                     -Wnoexcept \
                     -Wold-style-cast \
                     -Woverloaded-virtual \
                     -Wredundant-decls \
                     -Wshadow \
                     -Wsign-promo \
                     -Wstrict-null-sentinel \
                     -Wstrict-overflow=5 \
                     -Wswitch-default \
                     -Wno-unused \
                     -Wno-variadic-macros \
                     -Wno-parentheses \
                     -fdiagnostics-show-option ${CMAKE_CXX_FLAGS}")

include(googletest)

enable_testing()


add_subdirectory(viperboard)
add_subdirectory(example)
