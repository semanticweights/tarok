#!/bin/bash
set -e

BUILD_DIR = "$(pwd)/../build"

# delete the build directory if it already exists
if [ -d ${BUILD_DIR} ]; then
    rm -rf ${BUILD_DIR}
fi

# build the project with cmake
mkdir ${BUILD_DIR}
cd ${BUILD_DIR}
cmake ../
make

exit 0
