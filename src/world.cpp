/**
 * world.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include "world.h"

namespace raytracer {

    std::ostream &operator<<(std::ostream &os, const world &world) {
        return os << "world(gravity: " << world.gravity << ", wind: " << world.wind << ')';
    }
}