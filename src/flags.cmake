target_link_libraries(${APP_NAME} csound64 csnd6)

include_directories(cuttlebone phasespace /usr/local/include ${CMAKE_SOURCE_DIR})

#find_library(CUTTLEBONE_LIB cuttlebone PATHS cuttlebone/build/ /usr/local/lib)
target_link_libraries("${APP_NAME}" ${CUTTLEBONE_LIB})
#message("User flags: ${CUTTLEBONE_LIB} ${APP_NAME}")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Wno-deprecated-declarations")
endif()

