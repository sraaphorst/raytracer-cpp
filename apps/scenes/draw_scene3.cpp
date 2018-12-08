/**
 * draw_scene3.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <fstream>
#include <vector>

#include <camera.h>
#include <canvas.h>
#include <constmath.h>
#include <material.h>
#include <plane.h>
#include <pointlight.h>
#include <ray.h>
#include <ringpattern.h>
#include <sphere.h>
#include <stripepattern.h>
#include <vec.h>
#include <world.h>

#include "../timer.h"

using namespace raytracer;
using namespace raytracer::shapes;

int main() {
    auto wall_material = std::make_shared<Material>();
    std::vector<Colour> wall_colours{predefined_colours::red, predefined_colours::white};
    auto wall_pattern = std::make_shared<StripePattern>(wall_colours);
    wall_pattern->setTransformation(scale(0.01, 0.01, 0.01) * rotation_y(math_constants::pi_by_three<>));
    wall_material->setPattern(wall_pattern);
    wall_material->setSpecular(0);

    std::vector<Colour> ball_colours{predefined_colours::white, predefined_colours::blue};
    auto ball_pattern = std::make_shared<StripePattern>(ball_colours);
    ball_pattern->setTransformation(scale(0.1, 0.2, 0.3));
    auto ball_material = std::make_shared<Material>(ball_pattern,
            Material::DEFAULT_AMBIENT, 0.7, 0.3, Material::DEFAULT_SHININESS);

    auto floor_material = std::make_shared<Material>();
    std::vector<Colour> floor_colours{make_colour(0.5, 1, 1), make_colour(1, 0.5, 1), make_colour(1, 1, 0)};
    auto floor_pattern = std::make_shared<RingPattern>(floor_colours);
    floor_pattern->setTransformation(scale(0.02, 0.03, 0.03));
    floor_material->setPattern(floor_pattern);
    floor_material->setSpecular(0);

    auto floor = Plane::createPlane();
    floor->setTransformation(scale(10, 0.01, 10));
    floor->setMaterial(floor_material);

    auto left_wall = Sphere::createSphere();
    left_wall->setTransformation(translation(0, 0, 5)
                                 * rotation_y(-math_constants::pi_by_four<>)
                                 * rotation_x(math_constants::pi_by_two<>)
                                 * scale(10, 0.01, 10));
    left_wall->setMaterial(wall_material);

    auto right_wall = Sphere::createSphere();
    right_wall->setTransformation(translation(0, 0, 5)
                                  * rotation_y(math_constants::pi_by_four<>)
                                  * rotation_x(math_constants::pi_by_two<>)
                                  * scale(10, 0.01, 10));
    right_wall->setMaterial(wall_material);

    auto middle_ball_material = std::make_shared<Material>();
    middle_ball_material->setReflectivity(1);
    auto middle_sphere = Sphere::createSphere();
    middle_sphere->setTransformation(translation(-0.5, 1, -0.5));
    middle_sphere->setMaterial(middle_ball_material);

    auto left_sphere = Sphere::createSphere();
    left_sphere->setTransformation(translation(-2, 0.33, -0.75) * scale(0.33, 0.33, 0.33));
    left_sphere->setMaterial(ball_material);

    auto right_sphere = Sphere::createSphere();
    right_sphere->setTransformation(translation(1.5, 0.5, -0.5) * scale(0.5, 0.5, 0.5));
    right_sphere->setMaterial(ball_material);

    std::vector<std::shared_ptr<Shape>> shapes = {floor, left_wall, right_wall,
                                                  left_sphere, right_sphere, middle_sphere};

    PointLight light{make_point(-10, 10, -10), predefined_colours::white};
    auto world = World{light, shapes};
    auto camera = Camera{1000, 500, math_constants::pi_by_three<>,
                         view_transform(make_point(0, 1.5, -5), make_point(0, 1, 0), predefined_tuples::y1)};

    run_timed("draw_scene3", [&camera, &world] {
        auto canvas = camera.render(world);
        std::ofstream out("scene3.ppm");
        out << canvas;
        out.close();
    });
}
