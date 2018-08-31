/**
 * TestCanvas.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "catch.hpp"

#include <array>
#include <memory>
#include <string>
#include <sstream>

#include "canvas.h"
#include "colour.h"

using namespace raytracer;

constexpr int width  = 10;
constexpr int height = 10;

TEST_CASE("Canvas initializes to black", "[Canvas][initialization]") {
    Canvas<width, height> c;
    for (auto i=0; i < width; ++i)
        for (auto j=0; j < height; ++j)
            REQUIRE(*(c[i][j]) == colour_constants::black);
}

TEST_CASE("Canvas can be written to", "[Canvas][set]") {
    Canvas<width, height> c;

    std::array<const Colour, 3> colours{
        colour_constants::red,
        colour_constants::green,
        colour_constants::blue
    };

    std::array<colour_ptr_t, 3> colour_ptrs;
    std::transform(colours.cbegin(), colours.cend(),
            colour_ptrs.begin(),
            [](auto c){ return colour_ptr_t{new Colour{c}}; });

    for (auto i=0; i < width; ++i)
        for (auto j=0; j < width; ++j)
            c[i][j] = colour_ptrs[(i + j) % 3];

    for (auto i=0; i < width; ++i)
        for (auto j=0; j < height; ++j)
            REQUIRE(*(c[i][j]) == colours[(i + j) % 3]);
}

TEST_CASE("Canvas outputs as PPM", "[Canvas][ppm]") {
    Canvas<5, 3> c;
    c[0][0] = colour_ptr_t{new Colour{ 1.5, 0  , 0}};
    c[2][1] = colour_ptr_t{new Colour{ 0  , 0.5, 0}};
    c[4][2] = colour_ptr_t{new Colour{-0.5, 0  , 1}};

    auto s = "P3\n5 3\n" + std::to_string(Colour::maxvalue) + '\n' +
            "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
            "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n";

    std::stringstream ostr;
    ostr << c;
    REQUIRE(s == ostr.str());
}

TEST_CASE("Canvas PPM file truncates at 70 characters", "[Canvas][ppm]") {
    Canvas<10, 2> c;
    colour_ptr_t colour_ptr{new Colour{1, 0.8, 0.6}};
    for (auto i=0; i < 10; ++i)
        for (auto j=0; j < 2; ++j)
            c[i][j] = colour_ptr;

    auto s = "P3\n10 2\n" + std::to_string(Colour::maxvalue) + '\n' +
            "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
            "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
            "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
            "153 255 204 153 255 204 153 255 204 153 255 204 153\n";

    std::stringstream ostr;
    ostr << c;
    REQUIRE(s == ostr.str());
}