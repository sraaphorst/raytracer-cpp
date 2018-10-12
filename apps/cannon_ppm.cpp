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
#include <vector.h>
#include <worldconditions.h>

using namespace raytracer;

int main() {
    WorldConditions w{make_vector(0, -0.1, 0), make_vector(-0.01, 0, 0)};
    std::unique_ptr<Projectile> ptr = std::make_unique<Projectile>(
            make_point(0, 1, 0),
            make_vector(1, 1.8, 0).normalize() * 11.25);
    Colour colour{1, 1, 0};

    Canvas c{900, 550};
    for (; ptr->inAir(); ptr = std::move(ptr->tick(w))) {
        auto pos = ptr->getPosition();
        auto x = (int) pos[tuple_constants::x];
        auto y = 550 - (int) pos[tuple_constants::y];

        assert(x >= 0 && x <= 900);
        assert(y >= 0 && y <= 550);

        c[x][y] = colour;
    }

    std::ofstream out("cannon.ppm");
    out << c;
    out.close();
}