cmake_minimum_required(VERSION 3.0)

project(BDSMNativeMods)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_SKIP_BUILD_RPATH TRUE)
SET(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w -s -std=c17 -Oz -fPIC -fPIE -fvisibility=hidden -Wl,--as-needed")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -s -std=c++2a -Oz -fPIC -fPIE -fvisibility=hidden")
SET(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
SET(LIBRARY_OUTPUT_DIRECTORY "mods")
INCLUDE_DIRECTORIES( ${INCLUDE_DIR} )

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

add_library(substrate SHARED stub.c)
set_target_properties(substrate PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/)

SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR}/src)

foreach(mod ${SUBDIRS})
  file(GLOB src "${PROJECT_SOURCE_DIR}/src/${mod}/*.cpp")
  add_library(${mod} SHARED ${src})
  target_link_libraries(${mod} PUBLIC substrate)
  if (EXISTS "${PROJECT_SOURCE_DIR}/src/${mod}/CMakeLists.txt")
    add_subdirectory(${PROJECT_SOURCE_DIR}/src/${mod})
  endif()
  set_target_properties(${mod} PROPERTIES PREFIX ""
                                          SUFFIX ".so"
                                          LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out/)
ENDFOREACH(mod ${SUBDIRS})