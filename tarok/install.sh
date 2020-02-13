#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" >/dev/null 2>&1; pwd -P)"
BUILD_DIR="${SCRIPT_DIR}/../build"

# install open spiel dependencies
echo "Installing open spiel dependencies"
cd ${SCRIPT_DIR}/libs/open_spiel
./install.sh
cd ${SCRIPT_DIR}

# delete the build directory if it already exists
if [ -d ${BUILD_DIR} ]; then
  rm -rf ${BUILD_DIR}
fi

echo "Building the tarok project"
# build the project
mkdir ${BUILD_DIR}
cd ${BUILD_DIR}
cmake ../tarok
make

# remind to add modules to python path
cd ..
ROOT_DIR=$(pwd)
echo "To add pyspiel and open_tarok to the python path add the following two commands to your .bashrc file:"
echo "export PYTHONPATH=\$PYTHONPATH:${ROOT_DIR}/build/libs/open_spiel"
echo "export PYTHONPATH=\$PYTHONPATH:${ROOT_DIR}/build/libs/open_spiel/open_spiel/python"
echo "export PYTHONPATH=\$PYTHONPATH:${ROOT_DIR}/build/src"

exit 0
