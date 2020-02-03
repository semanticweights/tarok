#!/bin/bash
set -e

WORKING_DIR=$(pwd)

# delete the build directory if it already exists
if [ -d "${WORKING_DIR}/build" ]; then
    rm -rf "${WORKING_DIR}/build"
fi

# build the project with cmake
mkdir "${WORKING_DIR}/build"
cd "${WORKING_DIR}/build"
cmake ../
make

exit 0
