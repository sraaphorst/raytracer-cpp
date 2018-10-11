# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(raytracer VERSION 0.1.0 LANGUAGES CXX)

add_library(raytracer SHARED
        canvas.h
        common.h
        constmath.h
        hit.h
        hit.cpp
        intersection.h
        intersection.cpp
        material.h
        material.cpp
        matrix.h
        meta.h
        pointlight.h
        projectile.h
        projectile.cpp
        ray.h
        shape.h
        sphere.h
        transformers.h
        vector.h
        variant_ops.h
        world.h
        world.cpp
        worldconditions.h
        worldconditions.cpp)
