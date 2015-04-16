#!/bin/sh

git clone -b devel --depth 1 https://github.com/AlloSphere-Research-Group/AlloSystem.git AlloSystem
git clone -b cmake --depth 1 https://github.com/AlloSphere-Research-Group/GLV.git GLV
git clone -b devel --depth 1 https://github.com/AlloSphere-Research-Group/Gamma.git Gamma

# Don't clone cuttlebone if on Windows.
if ! uname | grep 'MINGW' > /dev/null 2>&1; then
  git clone --depth 1 https://github.com/rbtsx/cuttlebone.git cuttlebone
fi

#git submodule add https://github.com/AlloSphere-Research-Group/vsr.git AlloSystem
