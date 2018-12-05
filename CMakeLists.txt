# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

cmake_minimum_required(VERSION 3.12)
project(raytracer_top)

set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-narrowing -ftemplate-depth=10000 -fconstexpr-depth=2000 -std=c++17 -g)
endif()

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(
        "${PROJECT_SOURCE_DIR}/src"
        "${PROJECT_SOURCE_DIR}/src/impl"
        "${PROJECT_SOURCE_DIR}/stc/patterns"
        "${PROJECT_SOURCE_DIR}/src/shapes"
)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(apps)
