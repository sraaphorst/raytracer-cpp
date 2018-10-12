/**
 * TestCamera.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "affine_transform.h"
#include "camera.h"
#include "canvas.h"
#include "common.h"
#include "constmath.h"
#include "matrix.h"
#include "ray.h"
#include "vector.h"
#include "world.h"

using namespace raytracer;

TEST_CASE("Constructing a camera") {
    const Camera c{160, 120, math_constants::pi_by_two<double>};
    REQUIRE(c.getHSize() == 160);
    REQUIRE(c.getVSize() == 120);
    REQUIRE(ALMOST_EQUALS(c.getFOV(), math_constants::pi_by_two<>));
    REQUIRE(c.getTransformation() == predefined_matrices::I<>);
}

TEST_CASE("The pixel size for a horizontal canvas") {
    const Camera c{200, 125, math_constants::pi_by_two<>};
    REQUIRE(ALMOST_EQUALS(c.getPixelSize(), 0.01));
}

TEST_CASE("The pixel size for a vertical canvas") {
    const Camera c{125, 200, math_constants::pi_by_two<>};
    REQUIRE(ALMOST_EQUALS(c.getPixelSize(), 0.01));
}

TEST_CASE("Construct a ray through the centre of the canvas") {
    const Camera c{201, 101, math_constants::pi_by_two<>};
    const auto r = c.ray_for_pixel(100, 50);
    REQUIRE(r.getOrigin() == predefined_tuples::zero_point);
    REQUIRE(r.getDirection() == make_vector(0, 0, -1));
}

TEST_CASE("Construct a ray through a corner of the canvas") {
    const Camera c{201, 101, math_constants::pi_by_two<>};
    const auto r = c.ray_for_pixel(0, 0);
    REQUIRE(r.getOrigin() == predefined_tuples::zero_point);
    REQUIRE(r.getDirection() == make_vector(0.66519, 0.33259, -0.66851));
}

TEST_CASE("Construct a ray when the camera is transformed") {
    const Camera c{201, 101, math_constants::pi_by_two<>,
            rotation_y(math_constants::pi_by_four<>) * translation(0, -2, 5)};
    const auto r = c.ray_for_pixel(100, 50);
    const auto sqrt2_by_2 = sqrtd(2) / 2;
    REQUIRE(r.getOrigin() == make_point(0, 2, -5));
    REQUIRE(r.getDirection() == make_vector(sqrt2_by_2, 0, -sqrt2_by_2));
}

TEST_CASE("Rendering a world with a camera") {
    const auto w = World::getDefaultWorld();
    const auto from = make_point(0, 0, -5);
    const auto to   = predefined_tuples::zero_point;
    const auto up   = predefined_tuples::y1;
    const Camera c{11, 11, math_constants::pi_by_two<>, view_transform(from, to, up)};
    const auto image = c.render(w);
    REQUIRE(image[5][5] == make_colour(0.38066, 0.47583, 0.2855));

}