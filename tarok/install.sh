#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" >/dev/null 2>&1; pwd -P)"
BUILD_DIR="${SCRIPT_DIR}/../build"

echo "Downloading open spiel dependencies"
cd ${SCRIPT_DIR}/libs/open_spiel
# this needs to be in sync with libs/open_spiel/open_spiel/scripts/install.sh
# pybind
[[ -d "./pybind11" ]] || git clone -b 'v2.2.4' --single-branch --depth 1 https://github.com/pybind/pybind11.git
# bridge
[[ -d open_spiel/games/bridge/double_dummy_solver ]] || \
  git clone -b 'develop' --single-branch --depth 1 https://github.com/jblespiau/dds.git  \
  open_spiel/games/bridge/double_dummy_solver
# abseil
if [[ ! -d open_spiel/abseil-cpp ]]; then
  git clone -b '20200225.1' --single-branch --depth 1 https://github.com/abseil/abseil-cpp.git open_spiel/abseil-cpp
fi
cd ${SCRIPT_DIR}

echo "Building the tarok project"
if [ ! -d ${BUILD_DIR} ]; then
  mkdir ${BUILD_DIR}
fi
cd ${BUILD_DIR}
cmake ../tarok
make pytarok tarok_tests pyspiel

# remind to add modules to python path
cd ..
ROOT_DIR=$(pwd)
echo "To add pyspiel and pytarok to the python path add the following commands to your .profile file:"
echo "export PYTHONPATH=\$PYTHONPATH:${ROOT_DIR}/build/libs/open_spiel"
echo "export PYTHONPATH=\$PYTHONPATH:${ROOT_DIR}/build/libs/open_spiel/open_spiel/python"
echo "export PYTHONPATH=\$PYTHONPATH:${ROOT_DIR}/build/src"

exit 0
