set(camerautils_SOURCES camerautils.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src/camerautils)
include_directories("/usr/include/eigen3")

add_library(camerautils ${camerautils_SOURCES})
target_link_libraries(camerautils
)
