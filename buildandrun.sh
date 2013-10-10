# Change this to suit your needs
CMAKE_FLAGS="-DBUILD_ALLOGLV=1 -DBUILD_GLV=1 -DBUILD_VSR=0 -DBUILD_GAMMA=1"

# ------------------------------------------------
# You shouldn't need to touch the stuff below

FILENAME=$(basename "$fullfile")
FILENAME="${FILENAME%.*}"
TARGET=${FILENAME}_run
ALLOVSR_BUILD=0

if [ $# == 0 ]
then
    echo Aborting: You must provide a source filename or a directory
    exit 1
fi


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

#rm CMakeCache.txt
#rm AlloSystem/CMakeCache.txt
cmake . ${CMAKE_FLAGS} ${TARGET_FLAG} -DNO_EXAMPLES=1 -DNO_PROJECTS=1

make $TARGET -j4 $*
