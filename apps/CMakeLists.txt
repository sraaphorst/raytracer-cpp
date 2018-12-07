# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

add_executable(cannon cannon/cannon.cpp cannon/projectile.cpp cannon/worldconditions.cpp)
target_link_libraries(cannon PRIVATE raytracer OpenMP::OpenMP_CXX)

add_executable(cannon_ppm cannon/cannon_ppm.cpp cannon/projectile.cpp cannon/worldconditions.cpp)
target_link_libraries(cannon_ppm PRIVATE raytracer OpenMP::OpenMP_CXX)

add_executable(clock clock/clock.cpp)
target_link_libraries(clock PRIVATE raytracer OpenMP::OpenMP_CXX)

add_executable(draw_sphere scenes/draw_sphere.cpp)
target_link_libraries(draw_sphere PRIVATE raytracer OpenMP::OpenMP_CXX)

add_executable(draw_scene scenes/draw_scene.cpp)
target_link_libraries(draw_scene PRIVATE raytracer OpenMP::OpenMP_CXX)

add_executable(draw_scene2 scenes/draw_scene2.cpp)
target_link_libraries(draw_scene2 PRIVATE raytracer OpenMP::OpenMP_CXX)

add_executable(draw_scene3 scenes/draw_scene3.cpp)
target_link_libraries(draw_scene3 PRIVATE raytracer OpenMP::OpenMP_CXX)

add_executable(shadow_puppets scenes/shadow_puppets.cpp)
target_link_libraries(shadow_puppets PRIVATE raytracer OpenMP::OpenMP_CXX)

add_executable(reflect_refract scenes/reflect_refract.cpp)
target_link_libraries(reflect_refract PRIVATE raytracer OpenMP::OpenMP_CXX)

add_executable(uv_mapping scenes/uv_mapping.cpp)
target_link_libraries(uv_mapping PRIVATE raytracer OpenMP::OpenMP_CXX)