add_definitions(${HOUDINI_CXXFLAGS})

file(GLOB HB_SOURCES "HoudiniBifrost.cpp" "GEO_BifrostIOTranslator.cpp" "newDSO.cpp")

# libHoudiniBifrost
add_library(${PROJECT_NAME} SHARED ${HB_SOURCES})
target_link_libraries(${PROJECT_NAME} bifrostapi tbb HoudiniUT HoudiniGEO)
install(TARGETS ${PROJECT_NAME} DESTINATION ${HOME_HOUDINI_DSO})

# bif2geo
file(GLOB B2G_SOURCES "bif2geo.cpp")
add_executable("bif2geo" ${B2G_SOURCES})
target_link_libraries("bif2geo" HoudiniUT HoudiniGEO bifrostapi HoudiniBifrost)
install(TARGETS ${PROJECT_NAME} DESTINATION ${HOME_HOUDINI_DSO})
