add_executable(cannon cannon.cpp)
target_link_libraries(cannon raytracer)

add_executable(cannon_ppm cannon_ppm.cpp)
target_link_libraries(cannon_ppm raytracer)

add_executable(clock clock.cpp)
target_link_libraries(clock raytracer)

add_executable(draw_sphere draw_sphere.cpp)
target_link_libraries(draw_sphere raytracer)

add_executable(draw_scene draw_scene.cpp)
target_link_libraries(draw_scene raytracer)

add_executable(draw_scene2 draw_scene2.cpp)
target_link_libraries(draw_scene2 raytracer)

add_executable(draw_scene3 draw_scene3.cpp)
target_link_libraries(draw_scene3 raytracer)