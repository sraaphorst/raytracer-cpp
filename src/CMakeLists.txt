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
        patterns/checkerpattern.h
        patterns/checkerpattern.cpp
        patterns/gradientpattern.h
        patterns/gradientpattern.cpp
        material.h
        material.cpp
        matrix.h
        patterns/pattern.h
        patterns/pattern.cpp
        pointlight.h
        patterns/ringpattern.h
        patterns/ringpattern.cpp
        patterns/solidpattern.h
        patterns/solidpattern.cpp
        patterns/stripepattern.h
        patterns/stripepattern.cpp
        transformers.h
        vec.h
        world.h
        world.cpp

        impl/hit.cpp
        impl/instance_manager.cpp
        impl/intersection.cpp
        impl/ray.cpp

        shapes/cube.cpp
        shapes/plane.cpp
        shapes/shape.cpp
        shapes/sphere.cpp
        )
