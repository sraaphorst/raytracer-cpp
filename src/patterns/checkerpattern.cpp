/**
 * checkerpattern.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>
#include <cmath>

#include "checkerpattern.h"
#include "shape.h"
#include "sphere.h"
#include "vec.h"

namespace raytracer {
    CheckerPattern::CheckerPattern() noexcept:
            colour1{predefined_colours::white},
            colour2{predefined_colours::black} {}

    const Colour CheckerPattern::colourAtObject(const std::shared_ptr<const shapes::Shape> &shape,
                                         const Tuple &world_point) const noexcept {
        const auto sphere = std::dynamic_pointer_cast<const shapes::Sphere>(shape);
        const auto object_point = shape->getTransformationInverse() * world_point;
        if (sphere != nullptr) {
            // TECHNIQUE 1: Point in object space.
            // Find d, vector to origin. This should be normal.
            const auto d = (object_point - predefined_tuples::zero_point).normalize();

            // Calculate u and v in range [0,1] each.
            double u = 0.5 + std::atan2(d[tuple_constants::z], d[tuple_constants::x]) / (2 * math_constants::pi<>);
            double v = 0.5 - std::asin(tuple_constants::y) / math_constants::pi<>;

            // Now migrate to [0,2] x [0,2] and multiply by pattern inverse.
            // Ignore the last parameter as it shouldn't matter.
            // Problem: there is no latitude: only longitude.
            const auto vec = this->transformationInverse * make_vector(2 * u, 2 * v, 1);

            // TECHNIQUE 2:
            // Get normal.
//            const auto N = sphere->normalAt(object_point);

            // Maps to a [u, v] value in [0, 1] x [0, 1].
//            double u1 = asin(N[tuple_constants::x]) / math_constants::pi<> + 0.5;
//            double v2 = asin(N[tuple_constants::y]) / math_constants::pi<> + 0.5;

            return colourAt(vec);
//            const auto d = shape->getTransformationInverse() * (world_point - shape->getTransformation() * predefined_tuples::zero_point);
//            double u = 0.5 + std::atan2(d[tuple_constants::z], d[tuple_constants::x]) / 2 * math_constants::pi<>;
//            double v = 0.5 - std::asin(tuple_constants::y) / math_constants::pi<>;
        } else {
            const auto pattern_point = this->transformationInverse * object_point;
            return colourAt(pattern_point);
        }
    }

    // TODO: In order to do this with UV mapping for a sphere, we'll need to get the normal
    //  at point, and then switch directions to get a vector to the origin of the sphere.
    //  We will probably need some calculation here to get the vector v to the right length.
    //  Then we use the formulae on Wikipedia at:
    //  https://en.wikipedia.org/wiki/UV_mapping
    //  u = 0.5 + arctan2(d_z, d_x) / 2pi
    //  v = 0.5 - arcsin(d_y) / pi
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