#!/bin/bash

scons platform=linuxbsd CCFLAGS="-I$1/include" LINKFLAGS="-L$1/lib" -j2
# add verbose=yes for debugging
