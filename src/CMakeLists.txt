# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(raytracer VERSION 0.1.0 LANGUAGES CXX)

include_directories(
        impl
        shapes
        patterns
)

add_library(raytracer SHARED
        camera.h
        camera.cpp
        canvas.h
        canvas.cpp
        common.h
        constmath.h
        material.h
        material.cpp
        matrix.h
        pointlight.h
        transformers.h
        vec.h
        world.h
        world.cpp

        impl/hit.cpp
        impl/instance_manager.cpp
        impl/intersection.cpp
        impl/ray.cpp

        shapes/cube.cpp
        shapes/cylinder.cpp
        shapes/plane.cpp
        shapes/shape.cpp
        shapes/sphere.cpp

        patterns/checkerpattern.h
        patterns/checkerpattern.cpp
        patterns/gradientpattern.h
        patterns/gradientpattern.cpp
        patterns/pattern.h
        patterns/pattern.cpp
        patterns/ringpattern.h
        patterns/ringpattern.cpp
        patterns/solidpattern.h
        patterns/solidpattern.cpp
        patterns/stripepattern.h
        patterns/stripepattern.cpp
        )
