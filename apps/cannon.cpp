/**
 * cannon.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iomanip>
#include <iostream>
#include <memory>

#include <tup.h>
#include <projectile.h>
#include <world.h>

using namespace raytracer;

int main() {
    world w{tup::vector(0, -0.1, 0), tup::vector(-0.01, 0, 0)};
    std::unique_ptr<projectile> ptr = std::make_unique<projectile>(tup::point(0, 1, 0), tup::vector(1, 1, 0).normalize());

    std::cout << "In " << w << "\n\tbeginning with " << *ptr << "\n\n";

    size_t ticks = 0;
    std::cout << "Time" << std::setw(34) << "Velocity" << std::setw(42) << "Position" << '\n';
    for (; ptr->inAir(); ptr = ptr->tick(w), ++ticks) {
        const tup &position = ptr->getPosition();
        const tup &velocity = ptr->getVelocity();

        std::cout << std::fixed
                  << std::setw( 4)                                                << ticks
                  << std::setw(20) << '(' << std::setprecision(5)                 << velocity[tup::x] << ", "
                  << std::setw( 8)        << std::setprecision(5)                 << velocity[tup::y] << ", "
                  << std::setw( 0)        << std::setprecision(0)                 << velocity[tup::z] << ')'
                  << std::setw(20) << '(' << std::setprecision(5) << std::setw(8) << position[tup::x] << ", "
                  << std::setw( 7)                                                << position[tup::y] << ", "
                  << std::setw( 1)        << std::setprecision(0)                 << position[tup::z] << ")\n";
    }

    std::cout << "\nTotal time to hit ground: " << ticks << " ticks." << std::endl;
}