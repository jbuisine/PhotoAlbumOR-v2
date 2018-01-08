#To fill
include_directories(${PARADISEO_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(moPhotoAlbum moead-photoAlbum.cpp)
target_link_libraries(moPhotoAlbum ${PARADISEO_LIBRARIES})

