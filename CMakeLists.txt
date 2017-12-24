cmake_minimum_required(VERSION 3.2)

project(viperboard)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

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


add_subdirectory(viperboard)
