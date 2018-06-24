cmake_minimum_required(VERSION 3.2)

project(viperboard)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_definitions(-std=c++11)

include(codecoverage)
include(googletest)

# coverage
option(CREATE_COVERAGE_TARGETS "Create targets to run unit tests with coverage" OFF)
if (CREATE_COVERAGE_TARGETS)
    message(STATUS "Adding targets for measuring code coverage")
    
    append_coverage_compiler_flags()
    set(COVERAGE_EXCLUDES 'c++/*' 'x86_64-linux-gnu/*' '*/gmock/*' '*/gtest/*')

    setup_target_for_coverage(NAME coverage EXECUTABLE test_viperboard DEPENDENCIES test_viperboard)

endif()
# end coverage

add_subdirectory(viperboard)
