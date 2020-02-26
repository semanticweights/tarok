/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "src/tarok.h"

namespace tarok {

namespace py = pybind11;

PYBIND11_MODULE(pytarok, m) { m.def("new_tarok_game", &NewTarokGame); }

}  // namespace tarok
