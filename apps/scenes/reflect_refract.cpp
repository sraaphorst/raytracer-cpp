/**
 * reflect_refract.cpp
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
#include <sphere.h>
#include <patterns/checkerpattern.h>
#include <patterns/solidpattern.h>
#include <patterns/stripepattern.h>
#include <vec.h>
#include <world.h>

#include "../timer.h"

using namespace raytracer;
using namespace raytracer::shapes;

int main() {
    Camera camera{400, 200, math_constants::pi_by_three<>,
                  view_transform(make_point(-2.6, 1.5, -3.9), make_point(-0.6, 1, -0.8), predefined_tuples::y1)};
    PointLight light{make_point(-4.9, 4.9, -1), predefined_colours::white};

    // WALL MATERIAL
    Material wall_material{};
    std::vector<Colour> wall_material_stripe_colours{make_colour(0.45, 0.45, 0.45), make_colour(0.55, 0.55, 0.55)};
    auto wall_material_pattern = std::make_shared<StripePattern>(wall_material_stripe_colours);
    wall_material_pattern->setTransformation(rotation_y(math_constants::pi_by_two<>) * scale(0.25, 0.25, 0.25));
    wall_material.setPattern(wall_material_pattern);
    wall_material.setAmbient(0);
    wall_material.setDiffuse(0.4);
    wall_material.setSpecular(0);
    wall_material.setReflectivity(0.3);

    // CHECKERED FLOOR
    auto floor = Plane::createPlane();
    Material floor_material{};
    auto floor_material_pattern = std::make_shared<CheckerPattern>(make_colour(0.35, 0.35, 0.35), make_colour(0.65, 0.65, 0.65));
    floor_material.setPattern(floor_material_pattern);
    floor_material.setSpecular(0);
    floor_material.setReflectivity(0.4);
    floor->setMaterial(floor_material);
    floor->setTransformation(rotation_y(math_constants::pi<> / 10));

    // CEILING
    auto ceiling = Plane::createPlane();
    Material ceiling_material{};
    ceiling_material.setPattern(std::make_shared<SolidPattern>(make_colour(0.8, 0.8, 0.8)));
    ceiling_material.setAmbient(0.3);
    ceiling_material.setSpecular(0);
    ceiling->setMaterial(ceiling_material);
    ceiling->setTransformation(translation(0, 5, 0));

    // WEST WALL
    auto west_wall = Plane::createPlane();
    west_wall->setMaterial(wall_material);
    west_wall->setTransformation(translation(-5, 0, 0) * rotation_z(math_constants::pi_by_two<>) *
                                 rotation_y(math_constants::pi_by_two<>));

    // EAST WALL
    auto east_wall = Plane::createPlane();
    east_wall->setMaterial(wall_material);
    east_wall->setTransformation(translation(5, 0, 0) * rotation_z(math_constants::pi_by_two<>) *
                                 rotation_y(math_constants::pi_by_two<>));


    // NORTH WALL
    auto north_wall = Plane::createPlane();
    north_wall->setMaterial(wall_material);
    north_wall->setTransformation(translation(0, 0, 5) * rotation_x(math_constants::pi_by_two<>));

    // SOUTH WALL
    auto south_wall = Plane::createPlane();
    south_wall->setMaterial(wall_material);
    south_wall->setTransformation(translation(0, 0, 5) * rotation_x(math_constants::pi_by_two<>));


    // BACKGROUND BALLS
    auto bsphere1 = Sphere::createSphere();
    Material bsphere1_material{};
    bsphere1_material.setPattern(std::make_shared<SolidPattern>(make_colour(0.8, 0.5, 0.3)));
    bsphere1_material.setShininess(50);
    bsphere1->setMaterial(bsphere1_material);
    bsphere1->setTransformation(translation(4.6, 0.4, 1) * scale(0.4, 0.4, 0.4));

    auto bsphere2 = Sphere::createSphere();
    Material bsphere2_material{};
    bsphere2_material.setPattern(std::make_shared<SolidPattern>(make_colour(0.9, 0.4, 0.5)));
    bsphere2_material.setShininess(50);
    bsphere2->setMaterial(bsphere2_material);
    bsphere2->setTransformation(translation(4.7, 0.3, 0.4) * scale(0.3, 0.3, 0.3));

    auto bsphere3 = Sphere::createSphere();
    Material bsphere3_material{};
    bsphere3_material.setPattern(std::make_shared<SolidPattern>(make_colour(0.4, 0.9, 0.6)));
    bsphere3_material.setShininess(50);
    bsphere3->setMaterial(bsphere3_material);
    bsphere3->setTransformation(translation(-1, 0.5, 4.5) * scale(0.5, 0.5, 0.5));

    auto bsphere4 = Sphere::createSphere();
    Material bsphere4_material{};
    bsphere4_material.setPattern(std::make_shared<SolidPattern>(make_colour(0.4, 0.6, 0.9)));
    bsphere4_material.setShininess(50);
    bsphere4->setMaterial(bsphere4_material);
    bsphere4->setTransformation(translation(-1.7, 0.3, 4.7) * scale(0.3, 0.3, 0.3));


    // FOREGROUND BALLS
    auto red_sphere = Sphere::createSphere();
    Material red_sphere_material{};
    red_sphere_material.setPattern(std::make_shared<SolidPattern>(make_colour(1, 0.3, 0.2)));
    red_sphere_material.setSpecular(0.4);
    red_sphere_material.setShininess(5);
    red_sphere->setMaterial(red_sphere_material);
    red_sphere->setTransformation(translation(-0.6, 1, 0.6));

    auto blue_sphere = Sphere::createSphere();
    Material blue_sphere_material{};
    blue_sphere_material.setPattern(std::make_shared<SolidPattern>(make_colour(0, 0, 0.2)));
    blue_sphere_material.setAmbient(0);
    blue_sphere_material.setDiffuse(0.4);
    blue_sphere_material.setSpecular(0.9);
    blue_sphere_material.setShininess(300);
    blue_sphere_material.setReflectivity(0.9);
    blue_sphere_material.setTransparency(0.9);
    blue_sphere_material.setRefractiveIndex(1.5);
    blue_sphere->setMaterial(blue_sphere_material);
    blue_sphere->setTransformation(translation(0.6, 0.7, -0.6) * scale(0.7, 0.7, 0.7));

    auto green_sphere = Sphere::createSphere();
    Material green_sphere_material{};
    green_sphere_material.setPattern(std::make_shared<SolidPattern>(make_colour(0, 0.2, 0)));
    green_sphere_material.setAmbient(0);
    green_sphere_material.setDiffuse(0.4);
    green_sphere_material.setSpecular(0.9);
    green_sphere_material.setShininess(300);
    green_sphere_material.setReflectivity(0.9);
    green_sphere_material.setTransparency(0.9);
    green_sphere_material.setRefractiveIndex(1.5);
    green_sphere->setMaterial(green_sphere_material);
    green_sphere->setTransformation(translation(-0.7, 0.5, -0.8) * scale(0.5, 0.5, 0.5));

    std::vector<std::shared_ptr<Shape>> shapes = {
            floor, ceiling, west_wall, east_wall, north_wall, south_wall,
            bsphere1, bsphere2, bsphere3, bsphere4,
            red_sphere, green_sphere, blue_sphere
    };
    World world{light, shapes};

    run_timed("reflect_refract", [&camera, &world] {
        auto canvas = camera.render(world);
        std::ofstream out("reflect_refract.ppm");
        out << canvas;
        out.close();
    });
}
