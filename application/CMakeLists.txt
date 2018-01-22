#To fill
include_directories(${PARADISEO_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(moQAP FRRMAB-DK-qap.cpp)

target_link_libraries(moQAP ${PARADISEO_LIBRARIES})

