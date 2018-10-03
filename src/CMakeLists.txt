# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(raytracer VERSION 0.1.0 LANGUAGES CXX)

add_library(raytracer SHARED
        canvas.h
        common.h
        constmath.h
        matrix.h
        meta.h
        projectile.h
        projectile.cpp
        transformers.h
        vector.h
        world.h
        world.cpp
        ray.h
        sphere.h
        intersection.h
        variant_ops.h
        shape.h)
