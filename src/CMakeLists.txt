# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(raytracer VERSION 0.1.0 LANGUAGES CXX)

add_library(raytracer SHARED
        tup.cpp
        projectile.cpp
        world.cpp
        )
