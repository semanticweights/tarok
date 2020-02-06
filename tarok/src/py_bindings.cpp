#include "tarok.h"

#include "pybind11/pybind11.h"

namespace tarok {

namespace py = pybind11;

PYBIND11_MODULE(open_tarok, m) {
    py::class_<TarokGame>(m, "TarokGame")
        .def(py::init())
        .def("get_name", &TarokGame::GetName);
}

} // namespace tarok
