set(PYBIND11_CPP_STANDARD -std=c++1z)
if(APPLE)
  set(CMAKE_CXX_FLAGS "-w -undefined dynamic_lookup")
endif()

# look for the interpreter component so that the linker uses the correct
# libpython when a virtual environment is loaded
find_package(Python3 COMPONENTS Interpreter Development)
include_directories(SYSTEM ${Python3_INCLUDE_DIRS})

# python module
pybind11_add_module(pytarok tarok.cpp py_bindings.cpp)
# remove the 'lib' prefix from the binary
set_target_properties(pytarok PROPERTIES PREFIX "")
target_link_libraries(pytarok PRIVATE open_spiel_core pybind11)

# c++ library
add_library(tarok_lib STATIC tarok.cpp)
target_link_libraries(tarok_lib PUBLIC open_spiel_core)
