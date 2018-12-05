/**
 * cannon.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iomanip>
#include <iostream>
#include <memory>

#include "projectile.h"
#include <vec.h>
#include "worldconditions.h"

using namespace raytracer;

int main() {
    WorldConditions w{make_vector(0, -0.1, 0), make_vector(-0.01, 0, 0)};
    Projectile p{make_point(0, 1, 0), make_vector(1, 1, 0).normalize()};

    std::cout << "In " << w << "\n\tbeginning with " << p << "\n\n";

    size_t ticks = 0;
    std::cout << "Time" << std::setw(34) << "Velocity" << std::setw(42) << "Position" << '\n';
    for (; p.inAir(); p = p.tick(w), ++ticks) {
        const Tuple &position = p.getPosition();
        const Tuple &velocity = p.getVelocity();

        std::cout << std::fixed
                  << std::setw( 4)                                                << ticks
                  << std::setw(20) << '(' << std::setprecision(5)                 << velocity[tuple_constants::x] << ", "
                  << std::setw( 8)        << std::setprecision(5)                 << velocity[tuple_constants::y] << ", "
                  << std::setw( 0)        << std::setprecision(0)                 << velocity[tuple_constants::z] << ')'
                  << std::setw(20) << '(' << std::setprecision(5) << std::setw(8) << position[tuple_constants::x] << ", "
                  << std::setw( 7)                                                << position[tuple_constants::y] << ", "
                  << std::setw( 1)        << std::setprecision(0)                 << position[tuple_constants::z] << ")\n";
    }

    std::cout << "\nTotal time to hit ground: " << ticks << " ticks." << std::endl;
}