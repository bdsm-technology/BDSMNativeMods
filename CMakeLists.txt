cmake_minimum_required(VERSION 3.0)

project(BDSMNativeMods)

SET(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -std=c++2a -O3 -fPIC -fPIE ")
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

SUBDIRLIST(SUBDIRS ${PROJECT_SOURCE_DIR}/src)

foreach(mod ${SUBDIRS})
  file(GLOB src ${PROJECT_SOURCE_DIR}/src/${mod}/*)
  add_library(${mod} SHARED ${src})
  add_custom_command(TARGET ${mod} POST_BUILD COMMAND patchelf --add-needed libsubstrate.so ${PROJECT_BINARY_DIR}/out/${mod}.so)
  set_target_properties(${mod} PROPERTIES PREFIX ""
                                          SUFFIX ".so"
                                          LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out/)
ENDFOREACH(mod ${SUBDIRS})