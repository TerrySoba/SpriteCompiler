#!/bin/bash

set -e

script_dir=$(dirname "$0")

dosbox -c "mount d ${PWD}" \
       -c "mount c ${script_dir}/dos/dos_test_app" \
       -c "c:" \
       -c "dostest.exe d:\RESULT.BIN d:\compiled.spr 320 200 10 10" \
       -c "exit" \
       > /dev/null 2>&1
