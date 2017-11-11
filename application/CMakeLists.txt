#To fill
include_directories(${PARADISEO_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(projectName main.cpp)
target_link_libraries(projectName ${PARADISEO_LIBRARIES})
