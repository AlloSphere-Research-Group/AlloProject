#! /bin/bash
# Change this to suit your needs
CMAKE_FLAGS="-DBUILD_ALLOUTIL=1 -DBUILD_ALLOGLV=1 -DBUILD_GLV=1 -DBUILD_VSR=0 -DBUILD_GAMMA=1"

# ------------------------------------------------
FILENAME=`echo $1 |cut -d'.' -f1 | sed -e "s|/|_|g"`
ALLOVSR_BUILD=0

if [ -f $1 ]
then
  TARGET_FLAG="-DBUILD_APP_FILE=$1 -DBUILD_DIR=0"
elif [ -d $1 ]
then
  TARGET_FLAG="-DBUILD_APP_DIR=$1 -DBUILD_DIR=1"
else
  echo Aborting: $1 is neither a file nor directory
  exit 1
fi

./distclean
cmake . ${CMAKE_FLAGS} ${TARGET_FLAG} -GXcode -DNO_EXAMPLES=1 -DNO_PROJECTS=1
