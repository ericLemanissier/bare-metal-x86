#!/bin/sh
set -e
. ./headers.sh

export PATH=${PWD}/opt/cross/bin:${PATH}

for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done
