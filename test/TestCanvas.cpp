/**
 * TestCanvas.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <sstream>

#include "canvas.h"
#include "colour.h"

using namespace raytracer;

constexpr int width  = 10;
constexpr int height = 10;

TEST_CASE("canvas initializes to black", "[canvas][initialization]") {
    canvas<width, height> c;
    for (int i=0; i < width; ++i)
        for (int j=0; j < height; ++j)
            REQUIRE(*(c[i][j]) == colour_constants::black);
}

TEST_CASE("canvas can be written to", "[canvas][set]") {
    canvas<width, height> c;

    std::array<const colour, 3> colours{
        colour_constants::red,
        colour_constants::green,
        colour_constants::blue
    };

    std::array<colour_ptr_t, 3> colour_ptrs;
    std::transform(colours.cbegin(), colours.cend(),
            colour_ptrs.begin(),
            [](auto c){ return colour_ptr_t{new colour{c}}; });

    for (int i=0; i < width; ++i)
        for (int j=0; j < width; ++j)
            c[i][j] = colour_ptrs[(i + j) % 3];

    for (int i=0; i < width; ++i)
        for (int j=0; j < height; ++j)
            REQUIRE(*(c[i][j]) == colours[(i + j) % 3]);
}

TEST_CASE("canvas outputs as PPM", "[canvas][ppm]") {
    canvas<5, 3> c;
    c[0][0] = colour_ptr_t{new colour{ 1.5, 0  , 0}};
    c[2][1] = colour_ptr_t{new colour{ 0  , 0.5, 0}};
    c[4][2] = colour_ptr_t{new colour{-0.5, 0  , 1}};

    auto s = "P3\n5 3\n" + std::to_string(colour::maxvalue) + '\n' +
            "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255";

    std::stringstream ostr;
    ostr << c;
    REQUIRE(s == ostr.str());
}