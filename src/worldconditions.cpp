/**
 * worldconditions.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include "worldconditions.h"

namespace raytracer {

    std::ostream &operator<<(std::ostream &os, const WorldConditions &world) {
        return os << "world(gravity: " << world.gravity << ", wind: " << world.wind << ')';
    }
}