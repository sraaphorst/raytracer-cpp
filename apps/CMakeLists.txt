# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

add_executable(cannon cannon/cannon.cpp)
target_link_libraries(cannon raytracer)

add_executable(cannon_ppm cannon/cannon_ppm.cpp)
target_link_libraries(cannon_ppm raytracer)

add_executable(clock clock/clock.cpp)
target_link_libraries(clock raytracer)

add_executable(draw_sphere scenes/draw_sphere.cpp)
target_link_libraries(draw_sphere raytracer)

add_executable(draw_scene scenes/draw_scene.cpp)
target_link_libraries(draw_scene raytracer)

add_executable(draw_scene2 scenes/draw_scene2.cpp)
target_link_libraries(draw_scene2 raytracer)

add_executable(draw_scene3 scenes/draw_scene3.cpp)
target_link_libraries(draw_scene3 PRIVATE raytracer OpenMP::OpenMP_CXX)