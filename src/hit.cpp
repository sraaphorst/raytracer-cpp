/**
 * hit.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "hit.h"
#include "vector.h"

namespace raytracer {
    Hit::Hit(const Intersection &i, const Tuple &point, const Tuple &eyev,
             const Tuple &normalv, const Tuple &reflectv, bool inside):
             Intersection{i}, point{point}, eyev{eyev}, normalv{normalv}, reflectv{reflectv}, inside{inside} {}

    const Tuple &Hit::getPoint() const noexcept {
        return point;
    }

    const Tuple &Hit::getEyeVector() const noexcept {
        return eyev;
    }

    const Tuple &Hit::getNormalVector() const noexcept {
        return normalv;
    }

    const Tuple &Hit::getReflectVector() const noexcept {
        return reflectv;
    }

    const bool Hit::isInside() const noexcept {
        return inside;
    }
}
