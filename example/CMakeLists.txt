cmake_minimum_required(VERSION 3.2)

include_directories(${LIBUSB_INCLUDE_DIRS})
add_executable(example example.cpp)
target_include_directories(viperboard PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../viperboard/inc)
target_link_libraries(example viperboard)
