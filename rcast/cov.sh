#!/usr/bin/env bash

SCRIPT_DIR=$(dirname "$(readlink -f $0)")
cd ${SCRIPT_DIR}

cargo clean
cargo llvm-cov --open
