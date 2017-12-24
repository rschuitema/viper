# This file sets up gtest, gmock and gmock_main libraries

# We need thread support
find_package(Threads REQUIRED)

include(ExternalProject)
set(GTEST_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest-prefix/src/googletest-build/googlemock)

ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.8.0
    CMAKE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    # Disable install to not corrupt our own installer
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS ${GTEST_DIR}/libgmock.a
                     ${GTEST_DIR}/libgmock_main.a
                     ${GTEST_DIR}/gtest/libgtest.a
)

add_library(gtest STATIC IMPORTED)
SET_PROPERTY(TARGET gtest PROPERTY IMPORTED_LOCATION ${GTEST_DIR}/gtest/libgtest.a)

add_library(gmock STATIC IMPORTED)
SET_PROPERTY(TARGET gmock PROPERTY IMPORTED_LOCATION ${GTEST_DIR}/libgmock.a)

add_library(gmock_main STATIC IMPORTED)
SET_PROPERTY(TARGET gmock_main PROPERTY IMPORTED_LOCATION ${GTEST_DIR}/libgmock_main.a)

# If somebody wants to use Gtest and Gmock he needs to link against these libraries
set(GTEST_LIBRARIES gtest gmock gmock_main pthread)

# If somebody wants to use Gtest and Gmock he needs to add the following directories to its include path
ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE ${source_dir}/googletest/include
                  ${source_dir}/googlemock/include)

