add_definitions(${HOUDINI_CXXFLAGS})

file(GLOB SOURCES "*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME} bifrostapi tbb HoudiniUT HoudiniGEO )

install(TARGETS ${PROJECT_NAME} DESTINATION ${HOME_HOUDINI_DSO})
