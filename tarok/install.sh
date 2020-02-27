#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" >/dev/null 2>&1; pwd -P)"
BUILD_DIR="${SCRIPT_DIR}/../build"

echo "Installing open spiel dependencies"
cd ${SCRIPT_DIR}/libs/open_spiel
./install.sh
cd ${SCRIPT_DIR}

echo "Building the tarok project"
if [ ! -d ${BUILD_DIR} ]; then
  mkdir ${BUILD_DIR}
fi
cd ${BUILD_DIR}
cmake --target pytarok tarok_tests ../tarok
make

# remind to add modules to python path
cd ..
ROOT_DIR=$(pwd)
echo "To add pyspiel and pytarok to the python path add the following two commands to your .zshrc / .bashrc file:"
echo "export PYTHONPATH=\$PYTHONPATH:${ROOT_DIR}/build/libs/open_spiel"
echo "export PYTHONPATH=\$PYTHONPATH:${ROOT_DIR}/build/libs/open_spiel/open_spiel/python"
echo "export PYTHONPATH=\$PYTHONPATH:${ROOT_DIR}/build/src"

exit 0
