AlloProject
===

This repository contains some scripts to work with AlloSystem. 

You can clone this repository to a particular directory (e.g. my_new_project_dir) with:

    git clone https://github.com/AlloSphere-Research-Group/AlloProject.git my_new_project_dir

Initializing
===

To intialize the project you need to run:

	./initproject.sh

This will get and update the AlloSystem, GLV and Gamma submodules. It will also initialize the current directory as a git directory, so you can then commit to a remote repo.

Building
===

To build and run a file, you can do:

	./run.sh src/simpleApp.cpp

To build all the .cpp files inside a directory into a single application and run it:

	./run.sh src

XCode projects
===

To generate an Xcode project you can do:

	./makexcode.sh src/simpleApp.cpp

Or for all the files in a directory as a single application:

	./makexcode.sh src

