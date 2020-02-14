#include "tarok.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace tarok {

namespace py = pybind11;

PYBIND11_MODULE(pytarok, m) {
  m.def("new_tarok_game", &NewTarokGame);
}

} // namespace tarok
