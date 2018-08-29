/**
 * cannon_ppm.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>
#include <fstream>
#include <memory>

#include <canvas.h>
#include <projectile.h>
#include <tup.h>
#include <world.h>

using namespace raytracer;

int main() {
    world w{tup::vector(0, -0.1, 0), tup::vector(-0.01, 0, 0)};
    std::unique_ptr<projectile> ptr = std::make_unique<projectile>(
            tup::point(0, 1, 0),
            tup::vector(1, 1.8, 0).normalize() * 11.25);

    colour_ptr_t colour_ptr{new colour{1, 1, 0}};

    canvas<900, 550> c;
    for (; ptr->inAir(); ptr = ptr->tick(w)) {
        auto pos = ptr->getPosition();
        auto x = (int) pos[tup::x];
        auto y = 550 - (int) pos[tup::y];

        assert(x >= 0 && x <= 900);
        assert(y >= 0 && y <= 550);

        c[x][y] = colour_ptr;
    }

    std::ofstream str("cannon.ppm");
    str << c;
    str.close();
}