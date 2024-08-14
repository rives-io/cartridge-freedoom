#!/bin/sh
WAD=freedoom1.wad
cat $WAD > /dev/null # force file cache
if [ $# -eq 0 ]; then # no arguments
    exec ./doom.elf \
        -iwad $WAD \
        -skill 3 \
        -level 1 \
        -numlevelsquit 3 \
        -deathquit \
        -autoquit \
        -nomenu \
        -nocheat
else
    exec ./doom.elf -iwad $WAD "$@"
fi
