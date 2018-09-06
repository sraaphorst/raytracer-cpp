add_executable(cannon cannon.cpp)
target_link_libraries(cannon raytracer)

add_executable(cannon_ppm cannon_ppm.cpp)
target_link_libraries(cannon_ppm raytracer)

add_executable(clock clock.cpp)
target_link_libraries(clock raytracer)
