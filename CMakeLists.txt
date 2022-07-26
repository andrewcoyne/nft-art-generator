cmake_minimum_required(VERSION 3.20.0)
project(nft-art-generator VERSION 0.1.0)

include(CTest)
enable_testing()

# Include Libraries
include_directories(/usr/local/Cellar/boost/1.79.0_1/include) # Boost
include_directories(/usr/local/Cellar/opencv/4.5.5_3/include/opencv4) # OpenCV

add_executable(nft-art-generator app/Generator.cpp)
target_include_directories(nft-art-generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(nft-art-generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
