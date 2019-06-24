/**
 * cover.cpp
 *
 * By Sebastian Raaphorst, 2019.
 */

#include <fstream>
#include <memory>
#include <vector>

#include <camera.h>
#include <canvas.h>
#include <constmath.h>
#include <cube.h>
#include <material.h>
#include <pointlight.h>
#include <plane.h>
#include <shape.h>
#include <sphere.h>
#include <world.h>

#include "../timer.h"

using namespace raytracer;
using namespace raytracer::shapes;

int main() {
    const Camera camera{1000, 1000, 0.785,
                        view_transform(make_point(-6, 6, -10), make_point(6, 0, 6), make_point(-0.45, 1, 0))};
    const PointLight light{make_point(50, 100, -50), predefined_colours::white};

    // Materials
    auto white_material = std::make_shared<Material>(predefined_colours::white);
    white_material->setDiffuse(0.7);
    white_material->setAmbient(0.1);
    white_material->setSpecular(0.0);
    white_material->setReflectivity(0.1);

    auto blue_material = std::make_shared<Material>(make_colour(0.537, 0.831, 0.914));
    blue_material->setDiffuse(0.7);
    blue_material->setAmbient(0.1);
    blue_material->setSpecular(0.0);
    blue_material->setReflectivity(0.1);

    auto red_material = std::make_shared<Material>(make_colour(0.941, 0.322, 0.388));
    red_material->setDiffuse(0.7);
    red_material->setAmbient(0.1);
    red_material->setSpecular(0.0);
    red_material->setReflectivity(0.1);

    auto purple_material = std::make_shared<Material>(make_colour(0.373, 0.404, 0.550));
    purple_material->setDiffuse(0.7);
    purple_material->setAmbient(0.1);
    purple_material->setSpecular(0.0);
    purple_material->setReflectivity(0.1);

    const auto standard_transform = scale(0.5, 0.5, 0.5) * translation(1, -1, 1);
    const auto large_object = scale(3.5, 3.5, 3.5) * standard_transform;
    const auto medium_object = scale(3, 3, 3) * standard_transform;
    const auto small_object = scale(2, 2, 2) * standard_transform;

    // White backdrop for the scene.
    auto plane = Plane::createPlane();
    auto plane_material = std::make_shared<Material>(predefined_colours::white);
    plane_material->setAmbient(1.0);
    plane_material->setDiffuse(0);
    plane_material->setSpecular(0);
    plane->setMaterial(plane_material);
    plane->setTransformation(translation(0, 0, 500) * rotation_x(math_constants::pi_by_two<>));

   // Elements of the scene.
   auto sphere = Sphere::createSphere();
   auto sphere_material = std::make_shared<Material>(make_colour(0.373, 0.404, 0.550));
   sphere_material->setDiffuse(0.2);
   sphere_material->setAmbient(0);
   sphere_material->setSpecular(1);
   sphere_material->setShininess(200);
   sphere_material->setReflectivity(0.7);
   sphere_material->setTransparency(0.7);
   sphere_material->setRefractiveIndex(1.5);
   sphere->setMaterial(sphere_material);
   sphere->setTransformation(large_object);

   auto cube1 = Cube::createCube();
   cube1->setMaterial(white_material);
   cube1->setTransformation(translation(4, 0, 0) * medium_object);

   auto cube2 = Cube::createCube();
   cube2->setMaterial(blue_material);
   cube2->setTransformation(translation(8.5, 1.5, -0.5) * large_object);

   auto cube3 = Cube::createCube();
   cube3->setMaterial(red_material);
   cube3->setTransformation(translation(0, 0, 4) * large_object);

   auto cube4 = Cube::createCube();
   cube4->setMaterial(white_material);
   cube4->setTransformation(translation(4, 0, 4) * small_object);

   auto cube5 = Cube::createCube();
   cube5->setMaterial(purple_material);
   cube5->setTransformation(translation(7.5, 0.5, 4) * medium_object);

    auto cube6 = Cube::createCube();
    cube6->setMaterial(white_material);
    cube6->setTransformation(translation(-0.25, 0.25, 8) * medium_object);

    auto cube7 = Cube::createCube();
    cube7->setMaterial(blue_material);
    cube7->setTransformation(translation(4, 1, 7.5) * large_object);

    auto cube8 = Cube::createCube();
    cube8->setMaterial(red_material);
    cube8->setTransformation(translation(10, 2, 7.5) * medium_object);

    auto cube9 = Cube::createCube();
    cube9->setMaterial(white_material);
    cube9->setTransformation(translation(8, 2, 12) * small_object);

    auto cube10 = Cube::createCube();
    cube10->setMaterial(white_material);
    cube10->setTransformation(translation(20, 1, 9) * small_object);

    auto cube11 = Cube::createCube();
    cube11->setMaterial(blue_material);
    cube11->setTransformation(translation(-0.5, -5, 0.25) * large_object);

    auto cube12 = Cube::createCube();
    cube12->setMaterial(red_material);
    cube12->setTransformation(translation(4, -4, 0) * large_object);

    auto cube13 = Cube::createCube();
    cube13->setMaterial(white_material);
    cube13->setTransformation(translation(8.5, -4, 0) * large_object);

    auto cube14 = Cube::createCube();
    cube14->setMaterial(white_material);
    cube14->setTransformation(translation(0, -4, 4) * large_object);

    auto cube15 = Cube::createCube();
    cube15->setMaterial(purple_material);
    cube15->setTransformation(translation(-0.5, -4.5, 8) * large_object);

    auto cube16 = Cube::createCube();
    cube16->setMaterial(white_material);
    cube16->setTransformation(translation(0, -8, 4) * large_object);

    auto cube17 = Cube::createCube();
    cube17->setMaterial(white_material);
    cube17->setTransformation(translation(-0.5, -8.5, 8) * large_object);

    const std::vector<std::shared_ptr<Shape>> shapes = {
        plane, sphere,
        cube1, cube2, cube3, cube4, cube5, cube6, cube7, cube8, cube9,
        cube10, cube11, cube12, cube13, cube14, cube15, cube16, cube17
    };

    const World world{light, shapes};

    run_timed("cover", [&camera, &world] {
        const auto canvas = camera.render(world);
        std::ofstream out("cover.ppm");
        out << canvas;
        out.close();
    });
}