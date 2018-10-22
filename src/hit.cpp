/**
 * hit.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "hit.h"
#include "vector.h"

namespace raytracer {
    Hit::Hit(const Intersection &i, const Tuple &point, const Tuple &under_point,
             const Tuple &eyev, const Tuple &normalv, const Tuple &reflectv,
             bool inside, double n1, double n2):
             Intersection{i}, point{point}, under_point{under_point},
             eyev{eyev}, normalv{normalv}, reflectv{reflectv},
             inside{inside}, n1{n1}, n2{n2} {}

    const Tuple &Hit::getPoint() const noexcept {
        return point;
    }

    const Tuple &Hit::getUnderPoint() const noexcept {
        return under_point;
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

    double Hit::getN1() const noexcept {
        return n1;
    }

    double Hit::getN2() const noexcept {
        return n2;
    }

    bool Hit::isInside() const noexcept {
        return inside;
    }
}
