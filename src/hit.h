/**
 * hit.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "intersection.h"
#include "vector.h"

namespace raytracer {
    /**
     * A Hit is a subclass of Intersection that contains additional information:
     * 1. The point in world-space where the intersection occurred.
     * 2. The eye vector (pointing back to the camera / eye).
     * 3. The normal vector.
     * 4. Whether or not the intersection occurs inside the object (in which case, the normal is inverted to
     *    illuminate the surface properly).
     */
    class Hit final: public Intersection {
        const Tuple point;
        const Tuple eyev;
        const Tuple normalv;
        const Tuple reflectv;
        const bool inside;

    public:
        Hit() = delete;
        Hit(const Intersection &i, const Tuple &point, const Tuple &eyev,
                const Tuple &normalv, const Tuple &reflectv, bool inside);
        Hit(const Hit&) = default;
        Hit(Hit&&) = default;

        const Tuple &getPoint() const noexcept;

        const Tuple &getEyeVector() const noexcept;

        const Tuple &getNormalVector() const noexcept;

        const Tuple &getReflectVector() const noexcept;

        const bool isInside() const noexcept;
    };
}