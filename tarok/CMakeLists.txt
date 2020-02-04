cmake_minimum_required(VERSION 3.12)
project(tarok)

# require c++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

# look for the interpreter component so that the linker uses the correct
# libpython when a virtual environment is loaded
find_package(Python3 COMPONENTS Interpreter Development)

# files
file(GLOB SOURCES "src/*.cpp")

# target
# add_executable(tarok ${SOURCES})
add_library(tarok MODULE ${SOURCES})

# without this the binary is called libtarok.so
set_target_properties(tarok PROPERTIES PREFIX "")

# include
target_include_directories(tarok PRIVATE libs/open_spiel libs/open_spiel/pybind11/include)
include_directories(SYSTEM ${Python3_INCLUDE_DIRS})

# external libs
add_subdirectory(libs/open_spiel/open_spiel)
add_subdirectory(libs/open_spiel/pybind11)

target_link_libraries(tarok
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
    pybind11::module
    pybind11::embed
)
