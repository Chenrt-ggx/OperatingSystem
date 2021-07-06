#!/bin/bash
#run gxemul
make clean
make
/OSLAB/gxemul -E testmips -C R3000 -M 64 -V -d gxemul/fs.img gxemul/vmlinux
