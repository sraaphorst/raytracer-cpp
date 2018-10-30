/**
 * draw_scene.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <fstream>

#include "camera.h"
#include "canvas.h"
#include "constmath.h"
#include "material.h"
#include "pointlight.h"
#include "impl/ray.h"
#include "solidpattern.h"
#include "shapes/sphere.h"
#include "vec.h"
#include "world.h"

#include "../timer.h"

using namespace raytracer;

int main() {
    Material wall_material{};
    wall_material.setPattern(std::make_shared<SolidPattern>(make_colour(1, 0.9, 0.9)));
    wall_material.setSpecular(0);

    auto floor = std::make_shared<Sphere>(scale(10, 0.01, 10), wall_material);

    auto left_wall = std::make_shared<Sphere>(translation(0, 0, 5)
                     * rotation_y(-math_constants::pi_by_four<>)
                     * rotation_x(math_constants::pi_by_two<>)
                     * scale(10, 0.01, 10),
                     wall_material);

    auto right_wall = std::make_shared<Sphere>(translation(0, 0, 5)
                      * rotation_y(math_constants::pi_by_four<>)
                      * rotation_x(math_constants::pi_by_two<>)
                      * scale(10, 0.01, 10),
                      wall_material);

    auto middle_sphere = std::make_shared<Sphere>(translation(-0.5, 1, -0.5),
                         Material{make_colour(0.1, 1, 0.5), Material::DEFAULT_AMBIENT, 0.7, 0.3, Material::DEFAULT_SHININESS});

    auto left_sphere = std::make_shared<Sphere>(translation(-2, 0.33, -0.75) * scale(0.33, 0.33, 0.33),
                       Material{make_colour(1, 0.8, 0.1), Material::DEFAULT_AMBIENT, 0.7, 0.3, Material::DEFAULT_SHININESS});

    auto right_sphere = std::make_shared<Sphere>(translation(1.5, 0.5, -0.5) * scale(0.5, 0.5, 0.5),
                        Material{make_colour(0.5, 1, 0.1), Material::DEFAULT_AMBIENT, 0.7, 0.3, Material::DEFAULT_SHININESS});

    std::vector<std::shared_ptr<Shape>> shapes = {floor, left_wall, right_wall, left_sphere, right_sphere, middle_sphere};

    PointLight light{make_point(-10, 10, -10), predefined_colours::white};
    auto world = World{light, shapes};
    auto camera = Camera{1000, 500, math_constants::pi_by_three<>,
            view_transform(make_point(0, 1.5, -5), make_point(0, 1, 0), predefined_tuples::y1)};

    run_timed("draw_scene", [&camera, &world] {
        auto canvas = camera.render(world);
        std::ofstream out("scene.ppm");
        out << canvas;
        out.close();
    });
}