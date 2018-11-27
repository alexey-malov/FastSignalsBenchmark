#!/usr/bin/env bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

cd "${SCRIPT_DIR}"
git submodule update --init --recursive

mkdir -p "${SCRIPT_DIR}/benchmark-bin" && cd "${SCRIPT_DIR}/benchmark-bin"
cmake ../benchmark -DBENCHMARK_ENABLE_TESTING=OFF

