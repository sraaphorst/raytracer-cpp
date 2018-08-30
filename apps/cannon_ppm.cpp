/**
 * cannon_ppm.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>
#include <fstream>
#include <memory>

#include <canvas.h>
#include <colour.h>
#include <projectile.h>
#include <tuple.h>
#include <world.h>

using namespace raytracer;

int main() {
    world w{Tuple::vector(0, -0.1, 0), Tuple::vector(-0.01, 0, 0)};
    std::unique_ptr<projectile> ptr = std::make_unique<projectile>(
            Tuple::point(0, 1, 0),
            Tuple::vector(1, 1.8, 0).normalize() * 11.25);

    colour_ptr_t colour_ptr{new Colour{1, 1, 0}};

    canvas<900, 550> c;
    for (; ptr->inAir(); ptr = ptr->tick(w)) {
        auto pos = ptr->getPosition();
        auto x = (int) pos[Tuple::x];
        auto y = 550 - (int) pos[Tuple::y];

        assert(x >= 0 && x <= 900);
        assert(y >= 0 && y <= 550);

        c[x][y] = colour_ptr;
    }

    std::ofstream str("cannon.ppm");
    str << c;
    str.close();
}