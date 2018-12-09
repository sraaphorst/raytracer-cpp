/**
 * cylinders.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <fstream>
#include <memory>
#include <vector>

#include <camera.h>
#include <canvas.h>
#include <constmath.h>
#include <cylinder.h>
#include <material.h>
#include <plane.h>
#include <pointlight.h>
#include <sphere.h>
#include <checkerpattern.h>
#include <solidpattern.h>
#include <stripepattern.h>
#include <vec.h>
#include <world.h>

#include "../timer.h"

using namespace raytracer;
using namespace raytracer::shapes;

int main() {
    Camera camera{1600, 800, math_constants::pi<> / 10,
            view_transform(make_point(8, 3.5, -9), make_point(0, 0.3, 0), predefined_tuples::y1)};
    PointLight light{make_point(1, 6.9, -4.9), predefined_colours::white};

    // FLOOR
    auto floor = Plane::createPlane();
    auto floor_pattern = std::make_shared<CheckerPattern>(make_colour(0.5, 0.5, 0.5), make_colour(0.75, 0.75, 0.75));
    floor_pattern->setTransformation(rotation_y(0.3) * scale(0.25, 0.25, 0.25));
    auto floor_material = std::make_shared<Material>(floor_pattern);
    floor_material->setAmbient(0.2);
    floor_material->setDiffuse(0.9);
    floor_material->setSpecular(0);
    floor->setMaterial(floor_material);

    // CYLINDER1
    auto cylinder1 = Cylinder::createCylinder();
    cylinder1->setMinimumY(0);
    cylinder1->setMaximumY(0.75);
    cylinder1->setCapped(true);
    cylinder1->setTransformation(translation(-1, 0, 1) * scale(0.5, 1, 0.5));
    auto cylinder1_material = std::make_shared<Material>(make_colour(0, 0, 0.6));
    cylinder1_material->setDiffuse(0.1);
    cylinder1_material->setSpecular(0.9);
    cylinder1_material->setShininess(300);
    cylinder1_material->setReflectivity(0.9);
    cylinder1->setMaterial(cylinder1_material);

    // CONCENTRIC CYLINDERS
    auto concentric_cylinder1 = Cylinder::createCylinder();
    concentric_cylinder1->setMinimumY(0);
    concentric_cylinder1->setMaximumY(0.2);
    concentric_cylinder1->setTransformation(
            translation(1, 0, 0) *
            scale(0.8, 1, 0.8)
            );
    auto concentric_cylinder1_material = std::make_shared<Material>(make_colour(1, 1, 0.3));
    concentric_cylinder1_material->setAmbient(0.1);
    concentric_cylinder1_material->setDiffuse(0.8);
    concentric_cylinder1_material->setSpecular(0.9);
    concentric_cylinder1_material->setShininess(300);
    concentric_cylinder1->setMaterial(concentric_cylinder1_material);

    auto concentric_cylinder2 = Cylinder::createCylinder();
    concentric_cylinder2->setMinimumY(0);
    concentric_cylinder2->setMaximumY(0.3);
    concentric_cylinder2->setTransformation(
            translation(1, 0, 0) *
            scale(0.6, 1, 0.6)
            );
    auto concentric_cylinder2_material = std::make_shared<Material>(make_colour(1, 0.9, 0.4));
    concentric_cylinder2_material->setAmbient(0.1);
    concentric_cylinder2_material->setDiffuse(0.8);
    concentric_cylinder2_material->setSpecular(0.9);
    concentric_cylinder2_material->setShininess(300);
    concentric_cylinder2->setMaterial(concentric_cylinder2_material);

    auto concentric_cylinder3 = Cylinder::createCylinder();
    concentric_cylinder3->setMinimumY(0);
    concentric_cylinder3->setMaximumY(0.4);
    concentric_cylinder3->setTransformation(
            translation(1, 0, 0) *
            scale(0.4, 1, 0.4)
            );
    auto concentric_cylinder3_material = std::make_shared<Material>(make_colour(1, 0.8, 0.5));
    concentric_cylinder3_material->setAmbient(0.1);
    concentric_cylinder3_material->setDiffuse(0.8);
    concentric_cylinder3_material->setSpecular(0.9);
    concentric_cylinder3_material->setShininess(300);
    concentric_cylinder3->setMaterial(concentric_cylinder3_material);

    auto concentric_cylinder4 = Cylinder::createCylinder();
    concentric_cylinder4->setMinimumY(0);
    concentric_cylinder4->setMaximumY(0.5);
    concentric_cylinder4->setCapped(true);
    concentric_cylinder4->setTransformation(
            translation(1, 0, 0) *
            scale(0.2, 1, 0.2)
            );
    auto concentric_cylinder4_material = std::make_shared<Material>(make_colour(1, 0.7, 0.6));
    concentric_cylinder4_material->setAmbient(0.1);
    concentric_cylinder4_material->setDiffuse(0.8);
    concentric_cylinder4_material->setSpecular(0.9);
    concentric_cylinder4_material->setShininess(300);
    concentric_cylinder4->setMaterial(concentric_cylinder4_material);

    // DECORATIVE CYLINMDERS
    auto decorative_cylinder1 = Cylinder::createCylinder();
    decorative_cylinder1->setMinimumY(0);
    decorative_cylinder1->setMaximumY(0.3);
    decorative_cylinder1->setCapped(true);
    decorative_cylinder1->setTransformation(
            translation(0, 0, -0.75) *
            scale(0.05, 1, 0.05)
            );
    auto decorative_cylinder1_material = std::make_shared<Material>(predefined_colours::red);
    decorative_cylinder1_material->setAmbient(0.1);
    decorative_cylinder1_material->setDiffuse(0.9);
    decorative_cylinder1_material->setSpecular(0.9);
    decorative_cylinder1_material->setShininess(300);
    decorative_cylinder1->setMaterial(decorative_cylinder1_material);

    auto decorative_cylinder2 = Cylinder::createCylinder();
    decorative_cylinder2->setMinimumY(0);
    decorative_cylinder2->setMaximumY(0.3);
    decorative_cylinder2->setCapped(true);
    decorative_cylinder2->setTransformation(
            translation(0, 0, -2.25) *
            rotation_y(-0.15) *
            translation(0, 0, 1.5) *
            scale(0.05, 1, 0.05)
            );
    auto decorative_cylinder2_material = std::make_shared<Material>(make_colour(1, 1, 0));
    decorative_cylinder2_material->setAmbient(0.1);
    decorative_cylinder2_material->setDiffuse(0.9);
    decorative_cylinder2_material->setSpecular(0.9);
    decorative_cylinder2_material->setShininess(300);
    decorative_cylinder2->setMaterial(decorative_cylinder2_material);

    auto decorative_cylinder3 = Cylinder::createCylinder();
    decorative_cylinder3->setMinimumY(0);
    decorative_cylinder3->setMaximumY(0.3);
    decorative_cylinder3->setCapped(true);
    decorative_cylinder3->setTransformation(
            translation(0, 0, -2.25) *
            rotation_y(-0.3) *
            translation(0, 0, 1.5) *
            scale(0.05, 1, 0.05)
            );
    auto decorative_cylinder3_material = std::make_shared<Material>(predefined_colours::green);
    decorative_cylinder3_material->setAmbient(0.1);
    decorative_cylinder3_material->setDiffuse(0.9);
    decorative_cylinder3_material->setSpecular(0.9);
    decorative_cylinder3_material->setShininess(300);
    decorative_cylinder3->setMaterial(decorative_cylinder3_material);

    auto decorative_cylinder4 = Cylinder::createCylinder();
    decorative_cylinder4->setMinimumY(0);
    decorative_cylinder4->setMaximumY(0.3);
    decorative_cylinder4->setCapped(true);
    decorative_cylinder4->setTransformation(
            translation(0, 0, -2.25) *
            rotation_y(-0.45) *
            translation(0, 0, 1.5) *
            scale(0.05, 1, 0.05)
            );
    auto decorative_cylinder4_material = std::make_shared<Material>(make_colour(0, 1, 1));
    decorative_cylinder4_material->setAmbient(0.1);
    decorative_cylinder4_material->setDiffuse(0.9);
    decorative_cylinder4_material->setSpecular(0.9);
    decorative_cylinder4_material->setShininess(300);
    decorative_cylinder4->setMaterial(decorative_cylinder4_material);

    // GLASS CYLINDER
    auto glass_cylinder = Cylinder::createCylinder();
    glass_cylinder->setMinimumY(0.0001);
    glass_cylinder->setMaximumY(0.5);
    glass_cylinder->setCapped(true);
    glass_cylinder->setTransformation(
            translation(0, 0, -1.5) *
            scale(0.33, 1, 0.33)
            );
    //glass_cylinder->setCastsShadow(false);
    auto glass_cylinder_material = std::make_shared<Material>(make_colour(0.25, 0, 0));
    glass_cylinder_material->setDiffuse(0.1);
    glass_cylinder_material->setSpecular(0.9);
    glass_cylinder_material->setShininess(300);
    glass_cylinder_material->setReflectivity(0.9);
    glass_cylinder_material->setTransparency(0.9);
    glass_cylinder_material->setRefractiveIndex(1.5);
    glass_cylinder->setMaterial(glass_cylinder_material);

    std::vector<std::shared_ptr<Shape>> shapes = {
            floor, cylinder1,
            concentric_cylinder1, concentric_cylinder2, concentric_cylinder3, concentric_cylinder4,
            decorative_cylinder1, decorative_cylinder2, decorative_cylinder3, decorative_cylinder4,
            glass_cylinder
    };
    World world{light, shapes};

    run_timed("cylinders", [&camera, &world] {
        auto canvas = camera.render(world);
        std::ofstream out("cylinders.ppm");
        out << canvas;
        out.close();
    });
}