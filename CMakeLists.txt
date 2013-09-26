cmake_minimum_required (VERSION 2.8)
project (AlloApps)

add_subdirectory(AlloSystem)

if(BUILD_DIR)
  file(GLOB ALLOPROJECT_APP_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${BUILD_APP_DIR}/*.cpp)
  string(REPLACE "/" "_" APP_NAME ${BUILD_APP_DIR})
  set(SOURCE_DIR ${BUILD_DIR})
else()
  set(ALLOPROJECT_APP_SRC ${BUILD_APP_FILE})
  string(REPLACE "/" "_" APP_NAME ${BUILD_APP_FILE})
  get_filename_component(APP_NAME ${APP_NAME} NAME_WE) # Get name w/o extension
  get_filename_component(SOURCE_DIR ${BUILD_APP_FILE} PATH)
endif(BUILD_DIR)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)

if(EXISTS ${SOURCE_DIR)/flags.txt)
  file(READ ${SOURCE_DIR}/flags.txt EXTRA_COMPILER_FLAGS)
  message("WARNING: Using additional flags from /projects/${dir}/flags.txt: " ${EXTRA_COMPILER_FLAGS})
endif()

add_executable(${APP_NAME} EXCLUDE_FROM_ALL "${ALLOPROJECT_APP_SRC}")
target_compile_definitions(${APP_NAME} "${EXTRA_COMPILER_FLAGS}")

message("Target: ${APP_NAME}")
message("From sources: ${ALLOPROJECT_APP_SRC}")

# Dependencies (check if targets exist and set variables)
get_target_property(ALLOCORE_LIBRARY allocore LOCATION)
get_target_property(ALLOCORE_INCLUDE_DIR allocore ALLOCORE_INCLUDE_DIR)
get_target_property(ALLOCORE_LINK_LIBRARIES allocore ALLOCORE_LINK_LIBRARIES)
add_dependencies(${APP_NAME} allocore) 

if(TARGET Gamma)
  set(GAMMA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Gamma)
  get_target_property(GAMMA_LIBRARY Gamma LOCATION)
  add_dependencies(${APP_NAME} Gamma)
endif(TARGET Gamma)

if(TARGET GLV)
  set(GLV_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/GLV)
  get_target_property(GLV_LIBRARY GLV LOCATION)
  add_dependencies(${APP_NAME} GLV)
endif(TARGET GLV)

if(TARGET VSR)
  if(TARGET ALLOVSR)
    set(VSR_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vsr)
    set_target_properties(vsr PROPERTIES GLV_INCLUDE_DIR "${GLV_RELATIVE_DIR}"
      GLV_LIBRARY "${GLV_LIBRARY}")
    get_target_property(VSR_LIBRARY vsr LOCATION)
  add_dependencies(${APP_NAME} vsr)
  endif(TARGET ALLOVSR)
endif(TARGET VSR)

if(TARGET alloutil)
  get_target_property(ALLOUTIL_LIBRARY alloutil LOCATION)
  get_target_property(ALLOUTIL_INCLUDE_DIR alloutil ALLOUTIL_INCLUDE_DIR)
  get_target_property(ALLOUTIL_LINK_LIBRARIES alloutil ALLOUTIL_LINK_LIBRARIES)
  add_dependencies(${APP_NAME} alloutil)
endif(TARGET alloutil)

if(TARGET alloGLV)
  get_target_property(ALLOGLV_LIBRARY alloGLV LOCATION)
  get_target_property(ALLOGLV_INCLUDE_DIR alloGLV ALLOGLV_INCLUDE_DIR)
  get_target_property(ALLOGLV_LINK_LIBRARIES alloGLV ALLOGLV_LINK_LIBRARIES)
  add_dependencies(${APP_NAME} alloGLV)
endif(TARGET alloGLV)


# TODO copy resources to build directory

#file(GLOB ALLOPROJECT_APP_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${BUILD_APP_DIR}/*.*)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/AlloSystem/allocore/
  ${CMAKE_CURRENT_SOURCE_DIR}/AlloSystem/alloutil/
  ${CMAKE_CURRENT_SOURCE_DIR}/AlloSystem/alloGLV/
  ${CMAKE_CURRENT_SOURCE_DIR}/AlloSystem/allovsr/
  ${GAMMA_INCLUDE_DIR} )
#    message("Gamma : ${GAMMA_INCLUDE_DIRs}")
target_link_libraries(${APP_NAME}
  ${ALLOCORE_LIBRARY} ${ALLOUTIL_LIBRARY} ${ALLOGLV_LIBRARY}
  ${GAMMA_LIBRARY} ${GLV_LIBRARY} ${VSR_LIBRARY}
  ${ALLOCORE_LINK_LIBRARIES} ${ALLOUTIL_LINK_LIBRARIES} ${ALLOGLV_LINK_LIBRARIES})
#list(REMOVE_ITEM PROJECT_RES_FILES ${ALLOPROJECT_APP_SRC})
add_custom_target("${APP_NAME}_run"
  COMMAND "${APP_NAME}"
  DEPENDS "${APP_NAME}"
  WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
  SOURCES ${ALLOPROJECT_APP_SRC}
  COMMENT "Running: ${APP_NAME}")