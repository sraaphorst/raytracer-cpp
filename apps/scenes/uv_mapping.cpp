/**
 * uv_mapping.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <fstream>
#include <vector>

#include <camera.h>
#include <canvas.h>
#include <constmath.h>
#include <material.h>
#include <pointlight.h>
#include <sphere.h>
#include <patterns/checkerpattern.h>
#include <vec.h>
#include <world.h>

#include "../timer.h"

using namespace raytracer;
using namespace raytracer::shapes;

int main() {
//    Camera camera{800, 400, math_constants::pi_by_six<>,
//            view_transform(make_point(40, 0, -70), make_point(0, 0, -5), predefined_tuples::y1)};
    Camera camera{400, 400, math_constants::pi<> / 10,
            view_transform(make_point(0, 0, 0), make_point(0, 0, 1), predefined_tuples::y1)};

    PointLight light{make_point(0, 10, -10), predefined_colours::white};

    // Ball 1 material
    auto ball1_pattern = std::make_shared<CheckerPattern>(make_colour(0.3, 1.0, 0.3),
                                                          make_colour(1.0, 1.0, 0.3));
    //ball1_pattern->setTransformation(rotation_y(math_constants::pi_by_six<>) * scale(0.25, 0.25, 0.25));
    ball1_pattern->setTransformation(scale(0.2, 0.2, 0.2));
    auto ball1_material = std::make_shared<Material>(ball1_pattern);
    auto ball1 = Sphere::createSphere();
    ball1->setMaterial(ball1_material);
    ball1->setTransformation(translation(0, 0, 10));

    std::vector<std::shared_ptr<Shape>> shapes = { ball1 };
    World world{light, shapes};

    run_timed("uv_mapping", [&camera, &world] {
        auto canvas = camera.render(world);
        std::ofstream out("uv_mapping.ppm");
        out << canvas;
        out.close();
    });
}