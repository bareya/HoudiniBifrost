add_definitions(${HOUDINI_CXXFLAGS})

file(GLOB SOURCES "*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME} HoudiniUT HoudiniGEO)
