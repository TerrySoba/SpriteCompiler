#!/bin/bash

set -e

script_dir=$(dirname "$0")
open_watcom_dir=${script_dir}/../open_watcom

# decompress open_watcom.tar.xz id "opt/opwatcom" directory does not exist
if [ ! -d "${open_watcom_dir}/opt/opwatcom" ]; then
    echo "Decompressing open_watcom.tar.xz..."
    tar -xJf ${open_watcom_dir}/open_watcom.tar.xz -C ${open_watcom_dir}
fi

export WATCOM=${open_watcom_dir}/opt/opwatcom
export PATH=$WATCOM/binl:$PATH
export EDPATH=$WATCOM/eddat
export INCLUDE=$WATCOM/h

make