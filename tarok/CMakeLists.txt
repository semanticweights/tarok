cmake_minimum_required(VERSION 3.12)
project(open_tarok)

# require c++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)
set(PYBIND11_CPP_STANDARD -std=c++1z)

if(APPLE)
  set(CMAKE_CXX_FLAGS "-w -undefined dynamic_lookup")
endif()

# look for the interpreter component so that the linker uses the correct
# libpython when a virtual environment is loaded
find_package(Python3 COMPONENTS Interpreter Development)

file(GLOB SOURCES "src/*.cpp")

# pybind target
add_subdirectory(libs/open_spiel/pybind11)
pybind11_add_module(open_tarok ${SOURCES})
# remove the 'lib' prefix from the binary
set_target_properties(open_tarok PROPERTIES PREFIX "")

set(INCLUDE_DIRS libs/open_spiel libs/open_spiel/pybind11/include)
target_include_directories(open_tarok PRIVATE ${INCLUDE_DIRS})
include_directories(SYSTEM ${Python3_INCLUDE_DIRS})

add_subdirectory(libs/open_spiel/open_spiel)

target_link_libraries(open_tarok PRIVATE
    open_spiel_core
    games
    game_transforms
    open_spiel_query
    bridge_double_dummy_solver
    algorithms
    utils
    absl::container
    absl::flags
    absl::flags_parse
    absl::optional
    absl::random_random
    absl::str_format
    absl::strings
    absl::time
    pybind11
    pybind11::pybind11
)
