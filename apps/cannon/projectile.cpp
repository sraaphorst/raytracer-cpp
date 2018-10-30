/**
 * projectile.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <ostream>

#include "projectile.h"

namespace raytracer {

    std::ostream &operator<<(std::ostream &os, const Projectile &projectile) {
        return os << "projectile(position: " << projectile.position << ", velocity: " << projectile.velocity << ')';
    }
}