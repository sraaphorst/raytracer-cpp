# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(raytracer VERSION 0.1.0 LANGUAGES CXX)

include_directories(
        impl
        shapes
)

add_library(raytracer SHARED
        camera.h
        camera.cpp
        canvas.h
        canvas.cpp
        common.h
        constmath.h
        checkerpattern.h
        checkerpattern.cpp
        gradientpattern.h
        gradientpattern.cpp
        material.h
        material.cpp
        matrix.h
        meta.h
        pattern.h
        pattern.cpp
        pointlight.h
        ringpattern.h
        ringpattern.cpp
        solidpattern.h
        solidpattern.cpp
        stripepattern.h
        stripepattern.cpp
        transformers.h
        vec.h
        variant_ops.h
        world.h
        world.cpp

        impl/hit.cpp
        impl/intersection.cpp
        impl/ray.cpp

        shapes/cube.cpp
        shapes/plane.cpp
        shapes/shape.cpp
        shapes/shapefactory.cpp
        shapes/sphere.cpp
        )
