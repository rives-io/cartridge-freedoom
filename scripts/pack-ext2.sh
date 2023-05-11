#!/bin/sh

INODE_SIZE=8192
BLOCK_SIZE=4096
INPUT_TAR=$1
BYTES=$(wc -c $INPUT_TAR | cut -d ' ' -f1)
BLOCKS=$((($BYTES * 7 + $BLOCK_SIZE * 4 - 1) / ($BLOCK_SIZE * 4)))
OUTPUT=$2
LABEL=${3:-rootfs}
TMP_DIR=$(mktemp -d)
TMP_TAR=$(mktemp)

tar --directory $TMP_DIR \
    -pxf $INPUT_TAR && \
tar --format=gnu \
    --sort=name \
    --mtime=1970-01-01 \
    --numeric-owner \
    --owner=0 \
    --group=0 \
    --directory $TMP_DIR \
    -pcf $(realpath $TMP_TAR) . && \
genext2fs \
    --faketime \
    --allow-holes \
    --squash-uids \
    --bytes-per-inode $INODE_SIZE \
    --block-size $BLOCK_SIZE \
    --size-in-blocks $BLOCKS \
    --volume-label $LABEL \
    --tarball $TMP_TAR \
    $OUTPUT && \
chmod -R 755 $TMP_DIR
rm -rf $TMP_TAR $TMP_DIR
