echo "Enter project name"
read PROJECTNAME
echo "Enter project directory [Enter for current directory]"
read PROJECTDIR
if [ -z "$VAR" ]; then
    PROJECTDIR=.
fi
mkdir $PROJECTDIR/$PROJECTNAME
mkdir $PROJECTDIR/$PROJECTNAME/src

# Copy needed files
cp buildandrun.sh  $PROJECTDIR/$PROJECTNAME
cp CMakeLists.txt $PROJECTDIR/$PROJECTNAME
cp src/simpleApp.cpp $PROJECTDIR/$PROJECTNAME/src

cd $PROJECTDIR/$PROJECTNAME


exit 1

# get AlloSystem as a submodule
git init
git submodule add https://github.com/AlloSphere-Research-Group/AlloSystem.git AlloSystem
git submodule init
git submodule update

# get cmake branch. Not needed once merged
cd AlloSystem
git checkout cmake
cd ..
git add -A
git commit -m "Initial commit for project"


