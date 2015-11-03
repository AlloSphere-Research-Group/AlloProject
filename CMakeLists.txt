cmake_minimum_required (VERSION 2.8)
project (AlloApps)

if((NOT ALLOPROJECT_BUILD_DIR) AND (NOT ALLOPROJECT_BUILD_APP_FILE))
  message(FATAL_ERROR "AlloProject requires passing a file or a directory to cmake. See run.sh script")
endif((NOT ALLOPROJECT_BUILD_DIR) AND (NOT ALLOPROJECT_BUILD_APP_FILE))
message("Building Project ${CMAKE_PROJECT_NAME}")

set(ALLOSYSTEM_BUILD_DIR ${ALLOPROJECT_BUILD_DIR})
set(ALLOSYSTEM_BUILD_APP_DIR ${ALLOPROJECT_BUILD_APP_DIR})
set(ALLOSYSTEM_BUILD_APP_FILE ${ALLOPROJECT_BUILD_APP_FILE})

add_subdirectory(AlloSystem)

