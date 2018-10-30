/**
 * cannon_ppm.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>
#include <fstream>
#include <memory>
#include <iostream>

#include <canvas.h>
#include "projectile.h"
#include <vec.h>
#include "worldconditions.h"

using namespace raytracer;

int main() {
    constexpr auto width = 900;
    constexpr auto height = 550;
    WorldConditions w{make_vector(0, -0.1, 0), make_vector(-0.01, 0, 0)};
    const auto colour = make_colour(1, 1, 0);

    Canvas c{width, height};
    for (Projectile p{make_point(0, 1, 0), make_vector(1, 1.8, 0).normalize() * 11.25}; p.inAir(); p = p.tick(w)) {
        const auto pos = p.getPosition();
        const auto x = (int) pos[tuple_constants::x];
        const auto y = height - (int) pos[tuple_constants::y];

        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);

        c[x][y] = colour;
    }

    std::ofstream out("cannon.ppm");
    out << c;
    out.close();
}