cmake_minimum_required(VERSION 3.13.4)
project(tarok)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

include_directories(${CMAKE_SOURCE_DIR} libs/open_spiel)

add_subdirectory(libs/open_spiel/open_spiel)
# exclude unnecessary open spiel targets within the following subdirectories
set_property(DIRECTORY libs/open_spiel/open_spiel/examples PROPERTY EXCLUDE_FROM_ALL True)
set_property(DIRECTORY libs/open_spiel/open_spiel/games PROPERTY EXCLUDE_FROM_ALL True)
set_property(DIRECTORY libs/open_spiel/open_spiel/game_transforms PROPERTY EXCLUDE_FROM_ALL True)
set_property(DIRECTORY libs/open_spiel/open_spiel/tests PROPERTY EXCLUDE_FROM_ALL True)

add_subdirectory(libs/open_spiel/pybind11)
add_subdirectory(libs/googletest)

add_subdirectory(src)
add_subdirectory(test)
