cmake_minimum_required(VERSION 3.0)

project(HoudiniBifrost CXX)

add_definitions(-Wno-comment)

# HOUDINI
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules/FindHoudini)
find_package(Houdini)
include_directories(${HOUDINI_INCLUDE})
link_directories(${HOUDINI_LIBRARIES})

# MAYA
if(NOT DEFINED MAYA_DIR)
	message(FATAL_ERROR "MAYA_DIR cmake variable is not set.")
endif()
message(STATUS "Maya path:" ${MAYA_DIR})

find_path(MAYA_MGLOBAL MGlobal.h HINTS ${MAYA_DIR}/include/maya)
if(${MAYA_MGLOBAL} STREQUAL "MAYA_MGLOBAL-NOTFOUND")
        message(FATAL_ERROR "MGlobal.h header has not been found in ${MAYA_DIR}/include/maya")
endif()
message(STATUS "Maya MGlobal.h: " ${MAYA_MGLOBAL})
include_directories(${MAYA_DIR}/include)
link_directories(${MAYA_DIR}/lib)

set(BIFROST_DIR ${MAYA_DIR}/plug-ins/bifrost)

# BIFROST
if(NOT DEFINED BIFROST_DIR)
	message(FATAL_ERROR "BIFROST_DIR cmake variable is not set.")
endif()
message(STATUS "Bifrost: " ${BIFROST_DIR})

# Bifrsot header test
if(NOT EXISTS ${BIFROST_DIR}/include/modules/aminomath/api/aminomath/mat.h)
	message(FATAL_ERROR "mat.h not found")
endif()

# Add Bifrost dirs
include_directories(${BIFROST_DIR}/include/modules)
link_directories(${BIFROST_DIR}/lib)

execute_process(COMMAND ls ${BIFROST_DIR}/include/modules/ OUTPUT_VARIABLE BIFROST_MODULES)
string(REPLACE "\n" ";" BIFROST_MODULES ${BIFROST_MODULES})

foreach(module ${BIFROST_MODULES})
	include_directories(${BIFROST_DIR}/include/modules/${module}/api/)
endforeach()

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH ${BIFROST_DIR}/lib)

# Add src
add_subdirectory(src)
