#!/usr/bin/env bash

SCRIPT_DIR=$(dirname "$(readlink -f $0)")

cmake -S ${SCRIPT_DIR}/Src -B ${SCRIPT_DIR}/build
cmake --build ${SCRIPT_DIR}/build --verbose --parallel
cmake --build ${SCRIPT_DIR}/build --target coverage
