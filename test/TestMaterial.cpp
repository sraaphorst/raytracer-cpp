/**
 * TestMaterial.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <memory>

#include "constmath.h"
#include "material.h"
#include "pattern.h"
#include "pointlight.h"
#include "solidpattern.h"
#include "shapes/sphere.h"
#include "stripepattern.h"

using namespace raytracer;
using namespace raytracer::shapes;

TEST_CASE("Material: The default material") {
    const Material m;
    REQUIRE(*m.getPattern() == SolidPattern{predefined_colours::white});
    REQUIRE(ALMOST_EQUALS(m.getAmbient(), 0.1));
    REQUIRE(ALMOST_EQUALS(m.getDiffuse(), 0.9));
    REQUIRE(ALMOST_EQUALS(m.getSpecular(), 0.9));
    REQUIRE(m.getShininess() == 200);
}

TEST_CASE("Material: Lighting with the eye between the light and the surface") {
    const Material m;
    const auto position = predefined_tuples::zero_point;
    const auto eyev = make_vector(0, 0, -1);
    const auto normalv = make_vector(0, 0, -1);
    const PointLight light{make_point(0, 0, -10), predefined_colours::white};
    const auto result = m.lighting(light, Sphere::createSphere(), position, eyev, normalv, false);
    REQUIRE(result == make_colour(1.9, 1.9, 1.9));
}

TEST_CASE("Material: Lighting with the eye between light and surface, eye offset 45 deg") {
    const Material m;
    const auto position = predefined_tuples::zero_point;
    const auto sqrt2by2 = sqrtd(2) / 2;
    const auto eyev = make_vector(0, sqrt2by2, -sqrt2by2);
    const auto normalv = make_vector(0, 0, -1);
    const PointLight light{make_point(0, 0, -10), predefined_colours::white};
    const auto result = m.lighting(light, Sphere::createSphere(), position, eyev, normalv, false);
    REQUIRE(result == predefined_colours::white);
}

TEST_CASE("Material: Lighting with eye opposite surface, light offset 45 deg") {
    const Material m;
    const auto position = predefined_tuples::zero_point;
    const auto eyev = make_vector(0, 0, -1);
    const auto normalv = make_vector(0, 0, -1);
    const PointLight light{make_point(0, 10, -10), predefined_colours::white};
    const auto result = m.lighting(light, Sphere::createSphere(), position, eyev, normalv, false);
    REQUIRE(result == make_colour(0.7364, 0.7364, 0.7364));
}

TEST_CASE("Material: Lighting with eye in the path of the reflection vector") {
    const Material m;
    const auto position = predefined_tuples::zero_point;
    const auto sqrt2by2 = sqrtd(2) / 2;
    const auto eyev = make_vector(0, -sqrt2by2, -sqrt2by2);
    const auto normalv = make_vector(0, 0, -1);
    const PointLight light{make_point(0, 10, -10), predefined_colours::white};
    const auto result = m.lighting(light, Sphere::createSphere(), position, eyev, normalv, false);
    REQUIRE(result == make_colour(1.6364, 1.6364, 1.6364));
}

TEST_CASE("Material: Lighting with the light behind the surface") {
    const Material m;
    const auto position = predefined_tuples::zero_point;
    const auto eyev = make_vector(0, 0, -1);
    const auto normalv = make_vector(0, 0, -1);
    const PointLight light{make_point(0, 0, 10), predefined_colours::white};
    const auto result = m.lighting(light, Sphere::createSphere(), position, eyev, normalv, false);
    REQUIRE(result == make_colour(0.1, 0.1, 0.1));
}

TEST_CASE("Material: Lighting with the surface in shadow") {
    const Material m;
    const auto position = predefined_tuples::zero_point;
    const auto eyev = -predefined_tuples::z1;
    const auto normalv = -predefined_tuples::z1;
    const PointLight light{make_point(0, 0, -10), predefined_colours::white};
    const bool in_shadow = true;
    const auto result = m.lighting(light, Sphere::createSphere(), position, eyev, normalv, in_shadow);
    REQUIRE(result == make_colour(0.1, 0.1, 0.1));
}

TEST_CASE("Material: Lighting with a pattern applied") {
    const Material m{std::make_shared<StripePattern>(), 1, 0, 0, Material::DEFAULT_SHININESS};
    const auto eyev = make_vector(0, 0, -1);
    const auto normalv = make_vector(0, 0, -1);
    const PointLight light{make_point(0, 0, -10), predefined_colours::white};
    const auto c1 = m.lighting(light, Sphere::createSphere(), make_point(0.9, 0, 0), eyev, normalv, false);
    const auto c2 = m.lighting(light, Sphere::createSphere(), make_point(1.1, 0, 0), eyev, normalv, false);
    REQUIRE(c1 == predefined_colours::white);
    REQUIRE(c2 == predefined_colours::black);
}

TEST_CASE("Material: Reflectivity for the default material") {
    REQUIRE(Material{}.getReflectivity() == 0);
}
