# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(raytracer VERSION 0.1.0 LANGUAGES CXX)

add_library(raytracer SHARED
        camera.h
        camera.cpp
        canvas.h
        canvas.cpp
        common.h
        constmath.h
        gradientpattern.h
        gradientpattern.cpp
        hit.h
        hit.cpp
        intersection.h
        intersection.cpp
        material.h
        material.cpp
        matrix.h
        meta.h
        pattern.h
        pattern.cpp
        plane.h
        plane.cpp
        pointlight.h
        projectile.h
        projectile.cpp
        ray.h
        ringpattern.h
        ringpattern.cpp
        solidpattern.h
        solidpattern.cpp
        shape.h
        shape.cpp
        sphere.h
        sphere.cpp
        stripepattern.h
        stripepattern.cpp
        transformers.h
        vector.h
        variant_ops.h
        world.h
        world.cpp
        worldconditions.h
        worldconditions.cpp)
