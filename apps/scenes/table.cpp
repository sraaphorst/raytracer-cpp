/**
 * table.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <fstream>
#include <memory>
#include <vector>

#include <camera.h>
#include <canvas.h>
#include <constmath.h>
#include <cube.h>
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
    Camera camera{800, 400, math_constants::pi_by_four<>,
                  view_transform(make_point(8, 6, -8), make_point(0, 3, 0), predefined_tuples::y1)};
    PointLight light{make_point(0, 6.9, -5), make_colour(1, 1, 0.9)};

    // FLOOR / CEILING
    auto floor_ceiling = Cube::createCube();
    floor_ceiling->setTransformation(scale(20, 7, 20) * translation(0, 1, 0));
    auto floor_ceiling_pattern = std::make_shared<CheckerPattern>(predefined_colours::black,
            make_colour(0.25, 0.25, 0.25));
    floor_ceiling_pattern->setTransformation(scale(0.07, 0.07, 0.07));
    auto floor_ceiling_material = std::make_shared<Material>(floor_ceiling_pattern);
    floor_ceiling_material->setAmbient(0.25);
    floor_ceiling_material->setDiffuse(0.7);
    floor_ceiling_material->setSpecular(0.9);
    floor_ceiling_material->setShininess(300);
    floor_ceiling_material->setReflectivity(0.1);
    floor_ceiling->setMaterial(floor_ceiling_material);

    // WALLS
    auto walls = Cube::createCube();
    walls->setTransformation(scale(10, 10, 10));
    auto wall_pattern = std::make_shared<CheckerPattern>(make_colour(0.4863, 0.3765, 0.2941),
            make_colour(0.3725, 0.2902, 0.2275));
    wall_pattern->setTransformation(scale(0.05, 20, 0.05));
    auto wall_material = std::make_shared<Material>(wall_pattern);
    wall_material->setAmbient(0.1);
    wall_material->setDiffuse((0.7));
    wall_material->setSpecular(0.9);
    wall_material->setShininess(300);
    wall_material->setReflectivity(0.05);
    walls->setMaterial(wall_material);

    // TABLETOP
    auto tabletop = Cube::createCube();
    tabletop->setTransformation(translation(0, 3.1, 0) * scale(3, 0.1, 2));
    std::vector<Colour> tabletop_stripes{make_colour(0.5529, 0.4235, 0.3255), make_colour(0.6588, 0.5098, 0.4000)};
    auto tabletop_pattern = std::make_shared<StripePattern>(tabletop_stripes);
    //tabletop_pattern->setTransformation(scale(0.05, 0.05, 0.05) * rotation_y(0.1));
    tabletop_pattern->setTransformation(scale(0.05, 0.05, 0.05) * rotation_y(0.0));
    auto tabletop_material = std::make_shared<Material>(tabletop_pattern);
    tabletop_material->setAmbient(0.1);
    tabletop_material->setDiffuse(0.7);
    tabletop_material->setSpecular(0.9);
    tabletop_material->setShininess(300);
    tabletop_material->setReflectivity(0.2);
    tabletop->setMaterial(tabletop_material);

    // LEGS
    auto leg_material = std::make_shared<Material>(std::make_shared<SolidPattern>(make_colour(0.5529, 0.4235, 0.3255)));
    leg_material->setAmbient(0.2);
    leg_material->setDiffuse(0.7);

    auto leg1 = Cube::createCube();
    leg1->setTransformation(translation(2.7, 1.5, -1.7) * scale(0.1, 1.5, 0.1));
    leg1->setMaterial(leg_material);

    auto leg2 = Cube::createCube();
    leg2->setTransformation(translation(2.7, 1.5, 1.7) * scale(0.1, 1.5, 0.1));
    leg2->setMaterial(leg_material);

    auto leg3 = Cube::createCube();
    leg3->setTransformation(translation(-2.7, 1.5, -1.7) * scale(0.1, 1.5, 0.1));
    leg3->setMaterial(leg_material);

    auto leg4 = Cube::createCube();
    leg4->setTransformation(translation(-2.7, 1.5, 1.7) * scale(0.1, 1.5, 0.1));
    leg4->setMaterial(leg_material);

    // GLASS CUBE
    auto glass_cube = Cube::createCube();
    glass_cube->setTransformation(translation(0, 3.45001, 0) * rotation_y(0.2) * scale(0.25, 0.25, 0.25));
    glass_cube->setCastsShadow(false);
    auto glass_cube_material = std::make_shared<Material>(std::make_shared<SolidPattern>(make_colour(1, 1, 0.8)));
    glass_cube_material->setAmbient(0);
    glass_cube_material->setDiffuse(0.3);
    glass_cube_material->setSpecular(0.9);
    glass_cube_material->setShininess(300);
    glass_cube_material->setReflectivity(0.7);
    glass_cube_material->setTransparency(0.7);
    glass_cube_material->setRefractiveIndex(1.5);
    glass_cube->setMaterial(glass_cube_material);

    // LITTLE CUBE #1
    auto little_cube1 = Cube::createCube();
    little_cube1->setTransformation(translation(1, 3.35, -0.9) * rotation_y(-0.4) * scale(0.15, 0.15, 0.15));
    auto little_cube1_material = std::make_shared<Material>(std::make_shared<SolidPattern>(make_colour(1, 0.5, 0.5)));
    little_cube1_material->setReflectivity(0.6);
    little_cube1_material->setDiffuse(0.4);
    little_cube1->setMaterial(little_cube1_material);

    // LITTLE CUBE #2
    auto little_cube2 = Cube::createCube();
    little_cube2->setTransformation(translation(-1.5, 3.27, 0.3) * rotation_y(0.4) * scale(0.15, 0.07, 0.15));
    auto little_cube2_material = std::make_shared<Material>(std::make_shared<SolidPattern>(make_colour(1, 1, 0.5)));
    little_cube2->setMaterial(little_cube2_material);

    // LITTLE CUBE #3
    auto little_cube3 = Cube::createCube();
    little_cube3->setTransformation(translation(0, 3.25, 1) * rotation_y(0.4) * scale(0.2, 0.05, 0.05));
    auto little_cube3_material = std::make_shared<Material>(std::make_shared<SolidPattern>(make_colour(0.5, 1, 0.5)));
    little_cube3->setMaterial(little_cube3_material);

    // LITTLE CUBE #4
    auto little_cube4 = Cube::createCube();
    little_cube4->setTransformation(translation(-0.6, 3.4, -1) * rotation_y(0.8) * scale(0.05, 0.2, 0.05));
    auto little_cube4_material = std::make_shared<Material>(std::make_shared<SolidPattern>(make_colour(0.5, 0.5, 1)));
    little_cube4->setMaterial(little_cube4_material);

    // LITTLE CUBE #5
    auto little_cube5 = Cube::createCube();
    little_cube5->setTransformation(translation(2, 3.4, 1) * rotation_y(0.8) * scale(0.05, 0.2, 0.05));
    auto little_cube5_material = std::make_shared<Material>(std::make_shared<SolidPattern>(make_colour(0.5, 1, 1)));
    little_cube5->setMaterial(little_cube5_material);

    // FRAME #1
    auto frame1 = Cube::createCube();
    frame1->setTransformation(translation(-10, 4, 1) * scale(0.05, 1, 1));
    auto frame1_material = std::make_shared<Material>(std::make_shared<SolidPattern>(make_colour(0.7098, 0.2471, 0.2196)));
    frame1_material->setDiffuse(0.6);
    frame1->setMaterial(frame1_material);

    // FRAME #2
    auto frame2 = Cube::createCube();
    frame2->setTransformation(translation(-10, 3.4, 2.7) * scale(0.05, 0.4, 0.4));
    auto frame2_material = std::make_shared<Material>(std::make_shared<SolidPattern>(make_colour(0.2667, 0.2706, 0.602)));
    frame2_material->setDiffuse(0.6);
    frame2->setMaterial(frame2_material);

    // FRAME #3
    auto frame3 = Cube::createCube();
    frame3->setTransformation(translation(-10, 4.6, 2.7) * scale(0.05, 0.4, 0.4));
    auto frame3_material = std::make_shared<Material>(std::make_shared<SolidPattern>(make_colour(0.3098, 0.5961, 0.3098)));
    frame3_material->setDiffuse(0.6);
    frame3->setMaterial(frame3_material);

    // MIRROR FRAME
    auto mirror_frame = Cube::createCube();
    mirror_frame->setTransformation(translation(-2, 3.5, 9.95) * scale(5, 1.5, 0.05));
    auto mirror_frame_material = std::make_shared<Material>(std::make_shared<SolidPattern>(make_colour(0.3882, 0.2627, 0.1882)));
    mirror_frame_material->setDiffuse(0.7);
    mirror_frame->setMaterial(mirror_frame_material);

    // MIRROR
    auto mirror = Cube::createCube();
    mirror->setTransformation(translation(-2, 3.5, 9.95) * scale(4.8, 1.4, 0.06));
    auto mirror_material = std::make_shared<Material>(std::make_shared<SolidPattern>(predefined_colours::black));
    mirror_material->setDiffuse(0);
    mirror_material->setAmbient(0);
    mirror_material->setSpecular(1);
    mirror_material->setShininess(300);
    mirror_material->setReflectivity(1);
    mirror->setMaterial(mirror_material);


    std::vector<std::shared_ptr<Shape>> shapes = {
            floor_ceiling, walls,
            tabletop, leg1, leg2, leg3, leg4,
            glass_cube, little_cube1, little_cube2, little_cube3, little_cube4, little_cube5,
            frame1, frame2, frame3, mirror_frame, mirror
    };
    World world{light, shapes};

    run_timed("table", [&camera, &world] {
        auto canvas = camera.render(world);
        std::ofstream out("table.ppm");
        out << canvas;
        out.close();
    });
}
