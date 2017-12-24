cmake_minimum_required(VERSION 3.2)

add_executable(test_viperboard test_viperboard.cpp)
target_include_directories(test_viperboard PUBLIC ${GTEST_INCLUDE})
target_link_libraries(test_viperboard ${GTEST_LIBRARIES})
add_test(Viperboard test_viperboard)
