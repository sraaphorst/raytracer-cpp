/**
 * TestWorld.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <optional>
#include <vector>

#include "affine_transform.h"
#include "constmath.h"
#include "hit.h"
#include "material.h"
#include "pattern.h"
#include "plane.h"
#include "pointlight.h"
#include "ray.h"
#include "solidpattern.h"
#include "sphere.h"
#include "vector.h"
#include "world.h"

#include "TestPattern.h"

using namespace raytracer;

TEST_CASE("World: Creating a world") {
    World w;
    REQUIRE(w.getObjects().empty());
    REQUIRE_FALSE(w.getLightSource().has_value());
}

TEST_CASE("World: The default world") {
    const PointLight light{make_point(-10, 10, -10), predefined_colours::white};

    const Material m1{make_colour(0.8, 1.0, 0.6), Material::DEFAULT_AMBIENT, 0.7, 0.2, Material::DEFAULT_SHININESS};
    Sphere s1{};
    s1.setMaterial(m1);

    Sphere s2{scale(0.5, 0.5, 0.5)};

    auto w = World::getDefaultWorld();
    REQUIRE(w.getLightSource().has_value());
    REQUIRE(w.getLightSource().value() == light);
    REQUIRE(w.contains(s1));
    REQUIRE(w.contains(s2));
}

TEST_CASE("World: Intersect a world with a ray") {
    auto w = World::getDefaultWorld();
    const Ray ray{make_point(0, 0, -5), make_vector(0, 0, 1)};
    auto xs = w.intersect(ray);
    REQUIRE(xs.size() == 4);
    REQUIRE(ALMOST_EQUALS(xs[0].getT(), 4));
    REQUIRE(ALMOST_EQUALS(xs[1].getT(), 4.5));
    REQUIRE(ALMOST_EQUALS(xs[2].getT(), 5.5));
    REQUIRE(ALMOST_EQUALS(xs[3].getT(), 6));
    REQUIRE(true);
}

TEST_CASE("World: Shading an intersection") {
    auto w = World::getDefaultWorld();
    const Ray ray{make_point(0, 0, -5), make_vector(0, 0, 1)};
    REQUIRE_FALSE(w.getObjects().empty());

    const auto s = w.getObjects().front();
    const Intersection i{4, s};
    auto hit = Intersection::prepareHit(i, ray, {});
    auto cOpt = w.shadeHit(hit);
    REQUIRE(cOpt.has_value());
    REQUIRE(cOpt.value() == make_colour(0.38066, 0.47583, 0.2855));
}

TEST_CASE("World: Shading an intersection from the inside") {
    auto w = World::getDefaultWorld();
    w.setLightSource(PointLight{make_point(0, 0.25, 0), predefined_colours::white});
    const Ray ray{make_point(0, 0, 0), make_vector(0, 0, 1)};
    REQUIRE(w.getObjects().size() >= 2);

    const auto s = w.getObjects()[1];
    const Intersection i{0.5, s};
    auto hit = Intersection::prepareHit(i, ray, {});
    auto cOpt = w.shadeHit(hit);
    REQUIRE(cOpt.has_value());

    /// After shadowing is implemented, we only have the ambient component.
    REQUIRE(cOpt.value() == make_colour(0.90498, 0.90498, 0.90498));
    /// REQUIRE(cOpt.value() == make_colour(0.1, 0.1, 0.1));
}

TEST_CASE("World: The colour when a ray misses") {
    const auto w = World::getDefaultWorld();
    const Ray ray{make_point(0, 0, -5), make_vector(0, 1, 0)};
    const auto c = w.colourAt(ray);
    REQUIRE(c == predefined_colours::black);
}

TEST_CASE("World: The colour when a ray hits") {
    const auto w = World::getDefaultWorld();
    const Ray ray{make_point(0, 0, -5), make_vector(0, 0, 1)};
    const auto c = w.colourAt(ray);
    REQUIRE(c == make_colour(0.38066, 0.47583, 0.2855));
}

TEST_CASE("World: The colour with an intersection behind the ray") {
    auto w = World::getDefaultWorld();
    REQUIRE(w.getObjects().size() >= 2);

    auto outer = w.getObjects()[0];
    auto mouter = outer->getMaterial();
    mouter.setAmbient(1);
    outer->setMaterial(mouter);

    auto inner = w.getObjects()[1];
    auto minner = inner->getMaterial();
    minner.setAmbient(1);
    inner->setMaterial(minner);

    REQUIRE(w.getObjects()[0]->getMaterial().getAmbient() == 1);
    REQUIRE(w.getObjects()[1]->getMaterial().getAmbient() == 1);
    Ray ray{make_point(0, 0, -0.75), make_vector(0, 0, 1)};
    REQUIRE(w.colourAt(ray) == inner->getMaterial().getPattern()->colourAt(make_point(0, 0, -0.75)));
}

TEST_CASE("World: There is no shadow when nothing is collinear with point and light") {
    const auto w = World::getDefaultWorld();
    const auto p = make_point(0, 10, 0);
    REQUIRE_FALSE(w.isShadowed(p));
}

TEST_CASE("World: Shadow when an object is between the point and the light") {
    const auto w = World::getDefaultWorld();
    const auto p = make_point(10, -10, 10);
    REQUIRE(w.isShadowed(p));
}

TEST_CASE("World: There is no shadow when an object is behind the light") {
    const auto w = World::getDefaultWorld();
    const auto p = make_point(-20, 20, -20);
    REQUIRE_FALSE(w.isShadowed(p));
}

TEST_CASE("World: There is no shadow when an object is behind the point") {
    const auto w = World::getDefaultWorld();
    const auto p = make_point(-2, -2, -2);
    REQUIRE_FALSE(w.isShadowed(p));
}

TEST_CASE("World: When shadeHit is given an intersection in shadow") {
    const PointLight light{make_point(0, 0, -10), predefined_colours::white};
    const std::shared_ptr<Shape> s1{std::make_shared<Sphere>()};
    const std::shared_ptr<Shape> s2{std::make_shared<Sphere>(translation(0, 0, 10))};
    const std::vector<std::shared_ptr<Shape>> shapes = {s1, s2};
    const World w{light, shapes};
    const Ray ray{make_point(0, 0, 5), predefined_tuples::z1};
    const Intersection i{4, s2};
    const auto hit = Intersection::prepareHit(i, ray, {});
    const auto c = w.shadeHit(hit);
    REQUIRE(c.has_value());
    REQUIRE(c.value() == make_colour(0.1, 0.1, 0.1));
}

TEST_CASE("World: Reflected colour for non-reflective material") {
    auto w = World::getDefaultWorld();
    const Ray ray{predefined_tuples::zero_point, predefined_tuples::z1};
    REQUIRE(w.getObjects().size() >= 2);
    auto &shape = w.getObjects()[1];
    shape->getMaterial().setAmbient(1);
    const Intersection x{1, shape};
    const auto hit = Intersection::prepareHit(x, ray, {});
    const auto colour = w.reflectedColour(hit);
    REQUIRE(colour == predefined_colours::black);
}

TEST_CASE("World: Reflected colour for reflective material") {
    auto w = World::getDefaultWorld();
    std::shared_ptr<Shape> shape = std::make_shared<Plane>(translation(0, -1, 0));
    shape->getMaterial().setReflectivity(0.5);
    const auto sqrt2    = sqrtd(2);
    const auto sqrt2by2 = sqrt2/2;
    const Ray ray{make_point(0, 0, -3), make_vector(0, -sqrt2by2, sqrt2by2)};
    const Intersection x{sqrt2, shape};
    const auto hit = Intersection::prepareHit(x, ray, {});
    const auto colour = w.reflectedColour(hit);
    REQUIRE(colour == make_colour(0.19032, 0.2379, 0.14274));
}

TEST_CASE("World: shadeHit with reflective material") {
    auto w = World::getDefaultWorld();
    std::shared_ptr<Shape> shape = std::make_shared<Plane>(translation(0, -1, 0));
    shape->getMaterial().setReflectivity(0.5);
    const auto sqrt2    = sqrtd(2);
    const auto sqrt2by2 = sqrt2/2;
    const Ray ray{make_point(0, 0, -3), make_vector(0, -sqrt2by2, sqrt2by2)};
    const Intersection x{sqrt2, shape};
    const auto hit = Intersection::prepareHit(x, ray, {});
    const auto colour = w.shadeHit(hit);
    REQUIRE(colour == make_colour(0.87677, 0.92436, 0.82918));
}

TEST_CASE("World: colorAt with mutually reflective surfaces") {
    std::shared_ptr<Shape> lower = std::make_shared<Plane>(translation(0, -1, 0));
    lower->getMaterial().setReflectivity(1);

    std::shared_ptr<Shape> upper = std::make_shared<Plane>(translation(0, 1, 0));
    upper->getMaterial().setReflectivity(1);

    const std::vector<std::shared_ptr<Shape>> shapes{lower, upper};
    const World w{PointLight{predefined_tuples::zero_point, predefined_colours::white}, shapes};
    const Ray ray{predefined_tuples::zero_point, predefined_tuples::y1};
    REQUIRE_NOTHROW(w.colourAt(ray));
}

TEST_CASE("World: Reflected colour at maximum recursive depth") {
    auto w = World::getDefaultWorld();
    std::shared_ptr<Shape> shape = std::make_shared<Plane>(translation(0, -1, 0));
    shape->getMaterial().setReflectivity(0.5);
    const auto sqrt2    = sqrtd(2);
    const auto sqrt2by2 = sqrt2/2;
    const Ray ray{make_point(0, 0, -3), make_vector(0, -sqrt2by2, sqrt2by2)};
    const Intersection x{sqrt2, shape};
    const auto hit = Intersection::prepareHit(x, ray, {});
    const auto colour = w.reflectedColour(hit, 0);
    REQUIRE(colour == predefined_colours::black);
}

TEST_CASE("World: Refracted colour with opaque surface") {
    const auto w = World::getDefaultWorld();
    REQUIRE_FALSE(w.getObjects().empty());
    const auto &shape = w.getObjects()[0];
    const Ray ray{make_point(0, 0, -5), predefined_tuples::z1};
    const std::vector<Intersection> xs{Intersection{4, shape}, Intersection{6, shape}};
    const auto hit = Intersection::prepareHit(xs[0], ray, xs);
    const auto colour = w.refractedColour(hit, 5);
    REQUIRE(colour == predefined_colours::black);
}

TEST_CASE("World: Refracted colour at maximum recursive depth") {
    auto w = World::getDefaultWorld();
    REQUIRE_FALSE(w.getObjects().empty());

    auto &shape = w.getObjects()[0];
    shape->getMaterial().setTransparency(1.0);
    shape->getMaterial().setRefractiveIndex(1.5);

    const Ray ray{make_point(0, 0, -5), predefined_tuples::z1};

    const std::vector<Intersection> xs{Intersection{4, shape}, Intersection{6, shape}};
    const auto hit = Intersection::prepareHit(xs[0], ray, xs);
    const auto colour = w.refractedColour(hit, 0);
    REQUIRE(colour == predefined_colours::black);
}

TEST_CASE("World: Refracted colour under total internal reflection") {
    auto w = World::getDefaultWorld();
    REQUIRE_FALSE(w.getObjects().empty());

    auto &shape = w.getObjects()[0];
    shape->getMaterial().setTransparency(1.0);
    shape->getMaterial().setRefractiveIndex(1.5);

    const auto sqrt2by2 = sqrtd(2)/2;
    const Ray ray{make_point(0, 0, sqrt2by2), predefined_tuples::y1};
    const std::vector<Intersection> xs{Intersection{-sqrt2by2, shape}, Intersection{sqrt2by2, shape}};
    const auto hit = Intersection::prepareHit(xs[1], ray, xs);
    const auto colour = w.refractedColour(hit);
    REQUIRE(colour == predefined_colours::black);
}

TEST_CASE("World: Refracted colour with refracted ray") {
    auto w = World::getDefaultWorld();
    REQUIRE(w.getObjects().size() >= 2);

    std::shared_ptr<Pattern> pattern = std::make_shared<TestPattern>();
    auto &shape1 = w.getObjects()[0];
    shape1->getMaterial().setAmbient(1);
    shape1->getMaterial().setPattern(pattern);

    auto &shape2 = w.getObjects()[1];
    shape2->getMaterial().setTransparency(1);
    shape2->getMaterial().setRefractiveIndex(1.5);

    const Ray ray{make_point(0, 0, 0.1), predefined_tuples::y1};
    const std::vector<Intersection> xs{Intersection{-0.9899, shape1},
                                       Intersection{-0.4899, shape2},
                                       Intersection{ 0.4899, shape2},
                                       Intersection{ 0.9899, shape1}};

    auto hit = Intersection::prepareHit(xs[2], ray, xs);
    auto colour = w.refractedColour(hit, 5);
    REQUIRE(colour == make_colour(0, 0.99878, 0.04724));
}

TEST_CASE("World: shadeHit with transparent material") {
    auto w = World::getDefaultWorld();

    std::shared_ptr<Shape> plane = std::make_shared<Plane>();
    plane->setTransformation(translation(0, -1, 0));
    plane->getMaterial().setTransparency(0.5);
    plane->getMaterial().setRefractiveIndex(1.5);
    w.getObjects().emplace_back(plane);

    std::shared_ptr<Shape> ball = std::make_shared<Sphere>();
    ball->setTransformation(translation(0, -3.5, -0.5));
    ball->getMaterial().setPattern(std::make_shared<SolidPattern>(predefined_colours::red));
    ball->getMaterial().setAmbient(0.5);
    w.getObjects().emplace_back(ball);

    const auto sqrt2 = sqrtd(2);
    const auto sqrt2by2 = sqrt2/2;
    const Ray ray{make_point(0, 0, -3), make_vector(0, -sqrt2by2, sqrt2by2)};
    const std::vector<Intersection> xs{Intersection{sqrt2, plane}};
    const auto hit = Intersection::prepareHit(xs[0], ray, xs);

    const auto colour = w.shadeHit(hit);
    REQUIRE(colour == make_colour(0.93642, 0.68642, 0.68642));
}

TEST_CASE("World: shadeHit with reflective, transparent material") {
    auto w = World::getDefaultWorld();

    std::shared_ptr<Shape> plane = std::make_shared<Plane>();
    plane->setTransformation(translation(0, -1, 0));
    plane->getMaterial().setReflectivity(0.5);
    plane->getMaterial().setTransparency(0.5);
    plane->getMaterial().setRefractiveIndex(1.5);
    w.getObjects().emplace_back(plane);

    std::shared_ptr<Shape> ball = std::make_shared<Sphere>();
    ball->setTransformation(translation(0, -3.5, -0.5));
    ball->getMaterial().setPattern(std::make_shared<SolidPattern>(predefined_colours::red));
    ball->getMaterial().setAmbient(0.5);
    w.getObjects().emplace_back(ball);

    const auto sqrt2 = sqrtd(2);
    const auto sqrt2by2 = sqrt2/2;
    const Ray ray{make_point(0, 0, -3), make_vector(0, -sqrt2by2, sqrt2by2)};
    const std::vector<Intersection> xs{Intersection{sqrt2, plane}};
    const auto hit = Intersection::prepareHit(xs[0], ray, xs);

    const auto colour = w.shadeHit(hit);
    REQUIRE(colour == make_colour(0.93391, 0.69643, 0.69243));
}