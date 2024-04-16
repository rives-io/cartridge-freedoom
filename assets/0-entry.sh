#!/bin/sh
WAD=freedoom1.wad
cat $WAD > /dev/null # force file cache
if [ $# -eq 0 ]; then # no arguments
    exec ./doom.elf \
        -iwad $WAD \
        -skill 3 -warp 1 1 \
        -levelquit 3 \
        -deathquit \
        -autoquit \
        -nomenu
else
    exec ./doom.elf -iwad $WAD "$@"
fi
