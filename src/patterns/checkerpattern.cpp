/**
 * checkerpattern.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cmath>

#include "checkerpattern.h"
#include "vec.h"

namespace raytracer {
    CheckerPattern::CheckerPattern() noexcept:
            colour1{predefined_colours::white},
            colour2{predefined_colours::black} {}

    // TODO: In order to do this with UV mapping for a sphere, we'll need to get the normal
    //  at point, and then switch directions to get a vector to the origin of the sphere.
    //  We will probably need some calculation here to get the vector v to the right length.
    //  Then we use the formulae on Wikipedia at:
    //  https://en.wikipedia.org/wiki/UV_mapping
    //  u = 0.5 + arctan2(d_z, d_x) / 2pi
    //  v = 0.5 = arcsin(d_y) / pi
    //  Then we use the (u, v), coordinates to determine the colour in the equivalent
    //  2D manner as the 3D calculation below.
    const Colour CheckerPattern::colourAt(const Tuple &point) const noexcept {
        const auto idx =
                (static_cast<size_t>(floor(point[tuple_constants::x])) +
                static_cast<size_t>(floor(point[tuple_constants::y])) +
                static_cast<size_t>(floor(point[tuple_constants::z]))) % 2;
        return idx == 0 ? colour1 : colour2;

    }

    bool CheckerPattern::doCompare(const Pattern &other) const noexcept {
        const auto othercp = dynamic_cast<const CheckerPattern* const>(&other);
        return othercp != nullptr && colour1 == othercp->colour1 && colour2 == othercp->colour2;
    }
}