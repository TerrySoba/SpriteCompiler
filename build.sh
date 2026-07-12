#!/bin/bash

set -e

script_dir=$(dirname "$0")

mkdir -p "${script_dir}/build"

pushd "$script_dir/build"

cmake ..
cmake --build .

popd
