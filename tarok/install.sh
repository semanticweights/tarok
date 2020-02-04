#!/bin/bash
set -e

SCRIPT_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
BUILD_DIR="${SCRIPT_DIR}/../build"

# delete the build directory if it already exists
if [ -d ${BUILD_DIR} ]; then
    rm -rf ${BUILD_DIR}
fi

# build the project with cmake
mkdir ${BUILD_DIR}
cd ${BUILD_DIR}
cmake ../tarok
make

exit 0
