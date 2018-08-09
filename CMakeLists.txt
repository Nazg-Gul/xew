project(xew)

cmake_minimum_required (VERSION 2.6)
if (NOT CMAKE_VERSION VERSION_LESS 2.8.4)
  cmake_policy (SET CMP0017 NEW)
endif()

include(CMakeParseArguments)

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
message(STATUS "Project source dir = ${PROJECT_SOURCE_DIR}")
message(STATUS "Project build dir = ${CMAKE_BINARY_DIR}")

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

include_directories(include)

add_library(xew
  source/xew.c
  source/xew_util.c
  source/xew_x11.c
  source/xew_xcb.c
  source/xew_xkbcommon.c

  source/xew_util.h

  include/xew.h
  include/xew_x11.h
  include/xew_xcb.h
  include/xew_xkbcommon.h
)

macro(ADD_EXAMPLE module)
  add_executable(example_${module} example/example_${module}.c)
  target_link_libraries(example_${module} xew ${CMAKE_DL_LIBS})
endmacro()


ADD_EXAMPLE(x11)
ADD_EXAMPLE(xcb)
ADD_EXAMPLE(xkbcommon)
