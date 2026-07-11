#!/bin/bash

set -e

script_dir=$(dirname "$0")

dosbox -c "mount d ${PWD}" \
       -c "mount c ${script_dir}/dos/dos_test_app" \
       -c "c:" \
       -c "dostest.exe d:\buffer.bin d:\output.bin 320 200 10 10" \
       -c "exit"
