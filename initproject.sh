#!/bin/bash

git init

git submodule add --depth 1 https://github.com/AlloSphere-Research-Group/AlloSystem.git AlloSystem
git submodule add --depth 1 https://github.com/AlloSphere-Research-Group/GLV.git GLV
git submodule add --depth 1 https://github.com/AlloSphere-Research-Group/Gamma.git Gamma
git submodule add --depth 1 https://github.com/rbtsx/cuttlebone.git cuttlebone

# git clone -b devel --depth 1 https://github.com/AlloSphere-Research-Group/AlloSystem.git AlloSystem
# git clone -b cmake --depth 1 https://github.com/AlloSphere-Research-Group/GLV.git GLV
# git clone -b devel --depth 1 https://github.com/AlloSphere-Research-Group/Gamma.git Gamma
# git clone --depth 1 https://github.com/rbtsx/cuttlebone.git cuttlebone
#git submodule add https://github.com/AlloSphere-Research-Group/vsr.git AlloSystem

#git add -A
#git commit -m "Initial commit for project"

# Remove remote of AlloProject to avoid committing to it
# git remote set-url --push origin no_push


