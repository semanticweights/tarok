cmake_minimum_required(VERSION 3.12)
project(tarok)

# require c++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

# files
file(GLOB SOURCES "src/*.cpp")

# target
add_executable(tarok ${SOURCES})

# include
target_include_directories(tarok PRIVATE libs/open_spiel)

# external libs
add_subdirectory(libs/open_spiel/open_spiel)
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
)
