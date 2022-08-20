cmake_minimum_required(VERSION 3.20.0)
project(nft-art-generator VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(SOURCES
    src/DuplicateChecker.cpp
    src/ImageBuilder.cpp
    src/Layer.cpp
    src/Logger.cpp
    src/Manager.cpp
    src/Settings.cpp
)
set(LIBRARY_NAME generate)
add_library(${LIBRARY_NAME} OBJECT ${SOURCES})

find_package(OpenCV REQUIRED)
find_package(nlohmann_json 3.11.2 REQUIRED)
target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})
get_target_property(JSON_INCLUDE_DIRS nlohmann_json::nlohmann_json INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSON_INCLUDE_DIRS})

add_executable(nft-art-generator app/Generator.cpp)
target_link_libraries(nft-art-generator PRIVATE ${LIBRARY_NAME})
target_link_libraries(nft-art-generator PRIVATE ${OpenCV_LIBS})
target_link_libraries(nft-art-generator PRIVATE nlohmann_json::nlohmann_json)

