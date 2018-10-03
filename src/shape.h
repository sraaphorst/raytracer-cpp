/**
 * shape.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <vector>

#include "affine_transform.h"
#include "ray.h"


namespace raytracer {
    class Intersection;

    class Shape {
    public:
        Shape() : transformation{predefined_matrices::I<double, 4>} {}

        virtual const std::vector<Intersection> intersect(const Ray &r) const noexcept = 0;

        bool operator==(const Shape &other) const noexcept {
            return doCompare(other);
        }

        const Transformation &getTransformation() const {
            return transformation;
        }

        void setTransformation(const Transformation &t) {
            transformation = t;
        }

    protected:
        virtual bool doCompare(const Shape &other) const = 0;

        Transformation transformation;
    };
}
