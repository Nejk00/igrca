cmake_minimum_required(VERSION 3.30)
project(game)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)
set(SDL2_PATH "C:/Users/nejcg/CLionProjects/game/cmake-build-debug")
set(SDL2_image_PATH "C:/Users/nejcg/CLionProjects/game/cmake-build-debug" )

INCLUDE(FindPkgConfig)
INCLUDE(FetchContent)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR})

add_executable(game src/main.cpp
        src/game.cpp
        src/Texture.cpp
        src/Map.cpp
        src/Vector2D.cpp
        src/Collision.cpp
        src/ECS/ECS.cpp
        src/GameLogic.hpp
        src/GameLogic.cpp

)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} )




