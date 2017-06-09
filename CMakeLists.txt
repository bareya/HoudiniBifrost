cmake_minimum_required(VERSION 2.8)

project(HoudiniBifrost CXX)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules/FindHoudini)

find_package(Houdini)

message(${HODUINI_VERSION})


# BIFROST
if(NOT DEFINED BIFROST_DIR)
	message(FATAL_ERROR "BIFROST_DIR cmake variable is not set.")
endif()

message("Using Bifrost: " ${BIFROST_DIR})

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

# Add src
add_subdirectory(src)
