/**
 * shape.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <vector>

#include "affine_transform.h"
#include "shape.h"
#include "intersection.h"
#include "material.h"
#include "matrix.h"
#include "ray.h"

namespace raytracer {
    Shape::Shape() noexcept: transformation{predefined_matrices::I<double, 4>}, material{} {}

    const std::vector<Intersection> Shape::intersect(const Ray &r0) const noexcept {
        // Transform the ray to object space.
        const Ray r = r0.transform(getTransformation().invert());

        // Return the shape-dependent implementation results.
        return local_intersect(r);
    }

    bool Shape::operator==(const Shape &other) const noexcept {
        return typeid(*this) == typeid(other)
               && transformation == other.transformation
               && material == other.material
               && doCompare(other);
    }

    bool Shape::operator!=(const Shape &other) const noexcept {
        return !(*this == other);
    }
}