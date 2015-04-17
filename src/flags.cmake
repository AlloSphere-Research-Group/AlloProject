include_directories(cuttlebone phasespace /usr/local/include ${CMAKE_SOURCE_DIR})

#find_library(CUTTLEBONE_LIB cuttlebone PATHS cuttlebone/build/ /usr/local/lib)
target_link_libraries("${APP_NAME}" ${CUTTLEBONE_LIB})
#message("User flags: ${CUTTLEBONE_LIB} ${APP_NAME}")

# Use C++11 on Linux, Mac OS X, and Windows.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations")

# Use libc++ if on Mac OS X.
if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()
