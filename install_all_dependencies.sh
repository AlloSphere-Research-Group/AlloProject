#!/bin/bash
# AlloProject dependencies install script

if [ ! -d "AlloSystem/allcore" ]; then
    echo "Please run initproject.sh script before this one."
    exit 1
fi

AlloSystem/allocore/install_dependencies.sh
AlloSystem/alloutil/install_dependencies.sh
AlloSystem/allonect/install_dependencies.sh

ROOT=`pwd`
PLATFORM=`uname`
ARCH=`uname -m`
echo Installing for $PLATFORM $ARCH from $ROOT 

if [ `which apt-get` ]; then
	echo "Found apt-get"
	sudo apt-get cmake
	
elif [ `which port` ]; then
	echo "Found MacPorts"
	sudo port cmake

elif [ `which brew` ]; then
	echo "Found Homebrew"
	brew update
	brew install cmake

elif [ `uname | grep MINGW` ]; then
	echo "Found MinGW / MSYS"
	echo "Please install CMAKE manually"

else
	echo "Error: No suitable package manager found."
	echo "Error: Install apt-get, MacPorts, or Homebrew and try again."
fi

