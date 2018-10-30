/**
 * ray.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cassert>
#include <stdexcept>

#include "transformers.h"
#include "vec.h"
#include "affine_transform.h"

namespace raytracer::impl {
    /// Defines the concept of a ray, which has a point of origin and a direction associated with it.
    class Ray final {
    private:
        Tuple origin;
        Tuple direction;

    public:
        /**
         * Create a Ray at the origin with no direction.
         */
        Ray() noexcept;

        /**
         * Create a Ray at the specified origin with the given direction.
         * @param origin the origin of the ray
         * @param direction the direction of the ray
         */
        Ray(const Tuple &origin, const Tuple &direction);

        Ray(const Ray&) noexcept = default;
        Ray(Ray&&) noexcept = default;
        Ray &operator=(const Ray&) = default;

        /// Determine if two rays are the same.
        bool operator==(const Ray&) const noexcept;

        /// Determine if two rays are different.
        bool operator!=(const Ray&) const noexcept;

        /// Determine the position of this ray at time t.
        Tuple position(double) const noexcept;

        const Tuple &getOrigin() const noexcept;
        const Tuple &getDirection() const noexcept;

        /// Create a new Ray by applying a transformation to this one.
        const Ray transform(const Transformation&) const;

    protected:
        /// Subclass comparison implementations should override this method.
        virtual bool doCompare(const Ray &other) const noexcept;
    };
}
