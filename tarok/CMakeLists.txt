cmake_minimum_required(VERSION 3.13.4)
project(tarok)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

include_directories(${CMAKE_SOURCE_DIR} libs/open_spiel)

add_subdirectory(libs/open_spiel/open_spiel EXCLUDE_FROM_ALL)
add_subdirectory(libs/open_spiel/pybind11 EXCLUDE_FROM_ALL)
add_subdirectory(libs/googletest EXCLUDE_FROM_ALL)

add_subdirectory(src)
add_subdirectory(test)
