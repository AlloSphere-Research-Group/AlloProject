cmake_minimum_required (VERSION 2.8)
project(Oro)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(DEBUG_SUFFIX "_debug")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

message("Building Project ${CMAKE_PROJECT_NAME}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" "${CMAKE_CURRENT_SOURCE_DIR}/AlloSystem/cmake_modules")
include(cmake_modules/make_dep.cmake)

make_dep(cuttlebone)
make_dep(Gamma)
make_dep(GLV)

set(GAMMA_LIBRARY ${Gamma_LIBRARY})

# Don't have AlloSystem try to build Gamma or GLV
set(BUILD_GAMMA 0)
set(BUILD_GLV 0)
add_subdirectory(AlloSystem)

if((NOT ALLOPROJECT_BUILD_DIR) AND (NOT ALLOPROJECT_BUILD_APP_FILE))
  message(FATAL_ERROR "AlloProject requires passing a file or a directory to cmake. See run.sh script")
endif((NOT ALLOPROJECT_BUILD_DIR) AND (NOT ALLOPROJECT_BUILD_APP_FILE))

# External dependencies (Gamma and GLV)

# Run from path (Used in run.sh script)
if((ALLOPROJECT_BUILD_DIR) OR (ALLOPROJECT_BUILD_APP_FILE))
    set(BUILD_DIR ${ALLOPROJECT_BUILD_DIR})
    set(BUILD_APP_FILE ${ALLOPROJECT_BUILD_APP_FILE})
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/CMakeRunTargets.cmake")
    set(BUILD_EXAMPLES 0)
endif((ALLOPROJECT_BUILD_DIR) OR (ALLOPROJECT_BUILD_APP_FILE))

# TODO copy resources to build directory

#file(GLOB ALLOPROJECT_APP_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${ALLOPROJECT_BUILD_APP_DIR}/*.*)

include_directories(${ALLOCORE_DEP_INCLUDE_DIRS}
  ${ALLOUTIL_DEP_INCLUDE_DIR}
  ${ALLOGLV_INCLUDE_DIR}
  ${GLV_INCLUDE_DIR}
  ${GAMMA_INCLUDE_DIR} )


