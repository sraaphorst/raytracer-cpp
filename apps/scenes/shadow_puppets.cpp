/**
 * shadow_puppets.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * From: http://forum.raytracerchallenge.com/thread/2/shadow-puppets-scene-description
 */

#include <fstream>
#include <vector>

#include <camera.h>
#include <canvas.h>
#include <constmath.h>
#include <material.h>
#include <pointlight.h>
#include <sphere.h>
#include <patterns/solidpattern.h>
#include <vec.h>
#include <world.h>

#include "../timer.h"

using namespace raytracer;
using namespace raytracer::shapes;

int main() {
    Camera camera{800, 400, math_constants::pi_by_six<>,
            view_transform(make_point(40, 0, -70), make_point(0, 0, -5), predefined_tuples::y1)};
    PointLight light{make_point(0, 0, -100), predefined_colours::white};

    Material sphere_material{};
    sphere_material.setAmbient(0.2);
    sphere_material.setDiffuse(0.8);
    sphere_material.setSpecular(0.3);
    sphere_material.setShininess(200);

    Material wrist_material{sphere_material};
    wrist_material.setPattern(std::make_shared<SolidPattern>(make_colour(0.1, 1, 1)));

    Material thumb_and_palm_material{sphere_material};
    thumb_and_palm_material.setPattern(std::make_shared<SolidPattern>(make_colour(0.1, 0.1, 1)));

    Material index_material{sphere_material};
    index_material.setPattern(std::make_shared<SolidPattern>(make_colour(1, 1, 0.1)));

    Material middle_material{sphere_material};
    middle_material.setPattern(std::make_shared<SolidPattern>(make_colour(0.1, 1, 0.5)));

    Material ring_material{sphere_material};
    ring_material.setPattern(std::make_shared<SolidPattern>(make_colour(0.1, 1, 0.1)));

    Material pinky_material(sphere_material);
    pinky_material.setPattern(std::make_shared<SolidPattern>(make_colour(0.1, 0.5, 1)));

    // Backdrop onto which to cast the shadow.
    auto backdrop = Sphere::createSphere();
    Material backdrop_material{};
    backdrop_material.setPattern(std::make_shared<SolidPattern>(predefined_colours::white));
    backdrop_material.setAmbient(0);
    backdrop_material.setDiffuse(0.5);
    backdrop_material.setSpecular(0);
    backdrop->setMaterial(backdrop_material);
    backdrop->setTransformation(translation(0, 0, 20) * scale(200, 200, 0.01));

    // Wrist
    auto wrist = Sphere::createSphere();
    wrist->setMaterial(wrist_material);
    wrist->setTransformation(rotation_z(math_constants::pi_by_four<>) * translation(-4, 0, -21) * scale(3, 3, 3));

    // Palm
    auto palm = Sphere::createSphere();
    palm->setMaterial(thumb_and_palm_material);
    palm->setTransformation(translation(0, 0, -15) * scale(4, 3, 3));

    // Thumb
    auto thumb = Sphere::createSphere();
    thumb->setMaterial(thumb_and_palm_material);
    thumb->setTransformation(translation(-2, 2, -16) * scale(1, 3, 1));

    // Index finger
    auto index = Sphere::createSphere();
    index->setMaterial(index_material);
    index->setTransformation(translation(3, 2, -22) * scale(3, 0.75, 0.75));

    // Middle finger
    auto middle = Sphere::createSphere();
    middle->setMaterial(middle_material);
    middle->setTransformation(translation(4, 1, -19) * scale(3, 0.75, 0.75));

    // Ring finger
    auto ring = Sphere::createSphere();
    ring->setMaterial(ring_material);
    ring->setTransformation(translation(4, 0, -18) * scale(3, 0.75, 0.75));

    // Pinky finger
    auto pinky = Sphere::createSphere();
    pinky->setMaterial(pinky_material);
    pinky->setTransformation(translation(3, -1.5, -20) * rotation_z(-math_constants::pi<> / 10.0)
        * translation(1, 0, 0) * scale(2.5, 0.6, 0.6));

    std::vector<std::shared_ptr<Shape>> shapes = {backdrop, wrist, palm, thumb, index, middle, ring, pinky};
    World world{light, shapes};

    run_timed("shadow_puppets", [&camera, &world] {
        auto canvas = camera.render(world);
        std::ofstream out("shadow_puppets.ppm");
        out << canvas;
        out.close();
    });
}