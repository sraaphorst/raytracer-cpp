/**
 * hit.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "hit.h"
#include "vector.h"

namespace raytracer {
    const Tuple &Hit::getPoint() const noexcept {
        return point;
    }

    const Tuple &Hit::getEyeVector() const noexcept {
        return eyev;
    }

    const Tuple &Hit::getNormalVector() const noexcept {
        return normalv;
    }

    const bool Hit::isInside() const noexcept {
        return inside;
    }
}
