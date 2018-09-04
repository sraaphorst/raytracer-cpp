cmake_minimum_required(VERSION 3.12)
project(raytracer_top)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-narrowing -std=c++17")

include_directories("${PROJECT_SOURCE_DIR}/src")

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(apps)
