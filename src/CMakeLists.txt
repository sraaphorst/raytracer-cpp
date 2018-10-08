# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(raytracer VERSION 0.1.0 LANGUAGES CXX)

add_library(raytracer SHARED
        canvas.h
        common.h
        constmath.h
        material.h
        material.cpp
        matrix.h
        meta.h
        pointlight.h
        projectile.h
        projectile.cpp
        transformers.h
        vector.h
        worldconditions.h
        worldconditions.cpp
        ray.h
        shape.h
        sphere.h
        intersection.h
        variant_ops.h
        material.cpp
        world.h
        world.cpp)
