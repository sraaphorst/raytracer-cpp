/**
 * TestCanvas.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <memory>
#include <string>
#include <sstream>

#include "canvas.h"
#include "vec.h"

using namespace raytracer;

constexpr int width  = 10;
constexpr int height = 10;

TEST_CASE("Canvas: Canvas initializes to black") {
    Canvas c{width, height};
    for (auto i=0; i < width; ++i)
        for (auto j=0; j < height; ++j)
            REQUIRE(c[i][j] == predefined_colours::black);
}

TEST_CASE("Canvas: Canvas can be written to") {
    Canvas c{width, height};

    constexpr std::array<const Colour, 3> colours{
        predefined_colours::red,
        predefined_colours::green,
        predefined_colours::blue
    };

    for (auto i=0; i < width; ++i)
        for (auto j=0; j < width; ++j)
            c[i][j] = colours[(i + j) % 3];

    for (auto i=0; i < width; ++i)
        for (auto j=0; j < height; ++j)
            REQUIRE((c[i][j]) == colours[(i + j) % 3]);
}

TEST_CASE("Canvas: Canvas outputs as PPM") {
    Canvas c{5, 3};
    c[0][0] = make_colour( 1.5, 0  , 0);
    c[2][1] = make_colour( 0  , 0.5, 0);
    c[4][2] = make_colour(-0.5, 0  , 1);

    auto s = "P3\n5 3\n" + std::to_string(colour_constants::maxvalue) + '\n' +
            "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n";

    std::stringstream ostr;
    ostr << c;
    REQUIRE(s == ostr.str());
}

TEST_CASE("Canvas: Canvas PPM file truncates at 70 characters") {
    Canvas c{10, 2};
    const auto colour = make_colour(1, 0.8, 0.6);
    for (auto i=0; i < 10; ++i)
        for (auto j=0; j < 2; ++j)
            c[i][j] = colour;

    auto s = "P3\n10 2\n" + std::to_string(colour_constants::maxvalue) + '\n' +
            "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
            "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
            "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
            "153 255 204 153 255 204 153 255 204 153 255 204 153\n";

    std::stringstream ostr;
    ostr << c;
    REQUIRE(s == ostr.str());
}