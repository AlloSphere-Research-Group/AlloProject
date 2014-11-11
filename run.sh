#!/bin/bash

# ------------------------------------------------
# You shouldn't need to touch the stuff below

# Get the number of processors on OS X, linux, and (to-do) Windows.
NPROC=$(grep --count ^processor /proc/cpuinfo 2>/dev/null || sysctl -n hw.ncpu || 2)
# Save one core for the gui.
PROC_FLAG=$((NPROC - 1))

if [ $# == 0 ]
then
    echo Aborting: You must provide a source filename or a directory
    exit 1
fi

FILENAME=$(basename "$1")
DIRNAME=$(dirname "$1")

if [ ${DIRNAME} == "." ]
then
  FILENAME="${FILENAME%.*}"
else
  FILENAME="${DIRNAME//./_}_${FILENAME%.*}"
fi

echo FILENAME: ${FILENAME}
TARGET=${FILENAME//\//_}_run
echo TARGET: ${TARGET}

if [ -f $1 ]
then
  TARGET_FLAG="-DALLOPROJECT_BUILD_APP_FILE=$1 -DALLOPROJECT_BUILD_DIR=0"
elif [ -d $1 ]
then
  TARGET_FLAG="-DALLOPROJECT_BUILD_APP_DIR=$1 -DALLOPROJECT_BUILD_DIR=1"
else
  echo Aborting: $1 is neither a file nor directory
  exit 1
fi

cmake . ${CMAKE_FLAGS} ${TARGET_FLAG} -DNO_EXAMPLES=1 -Wno-dev
make $TARGET -j "$PROC_FLAG" $*
