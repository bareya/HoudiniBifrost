cmake_minimum_required(VERSION 2.8)

project(HoudiniBifrost CXX)



# HOUDINI
if(NOT DEFINED HOUDINI_DIR)
	set(HOUDINI_DIR $ENV{HFS})
endif()

if(NOT DEFINED HOUDINI_DIR)
	message(FATAL_ERROR  "HFS environment variable not found, or HOUDINI_DIR cmake variable is not set.")
endif()
message(STATUS "Houdini path: " ${HOUDINI_DIR})

if(NOT EXISTS ${HOUDINI_DIR}/bin/hcustom)
	message(FATAL_ERROR "hcustom is not found in directory " ${HOUDINI_DIR}/bin)
endif()
message(STATUS "hcustom found: " ${HOUDINI_DIR}/bin/hcustom)


# run hcustom and extract definitions
execute_process(COMMAND "${HOUDINI_DIR}/bin/hcustom" "-c" OUTPUT_VARIABLE HOUDINI_CXXFLAGS)
string(STRIP ${HOUDINI_CXXFLAGS} HOUDINI_CXXFLAGS)
string(REPLACE " " ";" HOUDINI_CXXFLAGS ${HOUDINI_CXXFLAGS})

execute_process(COMMAND "${HOUDINI_DIR}/bin/hcustom" "-m" OUTPUT_VARIABLE HOUDINI_LDFLAGS)
string(STRIP ${HOUDINI_LDFLAGS} HOUDINI_LDFLAGS)
string(REPLACE " " ";" HOUDINI_LDFLAGS ${HOUDINI_LDFLAGS})

# Houdini header test
set(HOUDINI_HEADER_TEST ${HOUDINI_DIR}/toolkit/include/GEO/GEO_IOTranslator.h)
if(NOT EXISTS ${HOUDINI_HEADER_TEST})
	message(FATAL_ERROR "${HOUDINI_HEADER_TEST} not found!")
endif()
message(STATUS "Houdini header test found: " ${HOUDINI_HEADER_TEST})

# Add Houdini dirs
include_directories(${HOUDINI_DIR}/toolkit/include)
link_directories(${HOUDINI_DIR}/dsolib)



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
