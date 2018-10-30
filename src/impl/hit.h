/**
 * hit.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <vec.h>
#include "intersection.h"

namespace raytracer::impl {
    /**
     * A Hit is a subclass of \see{Intersection} that contains additional information:
     * 1. The point in world-space where the intersection occurred, adjusted slightly to prevent acne.
     * 2. The "under point", i.e. the point adjusted slightly in the opposite direction for reflection.
     * 3. The eye vector (pointing back to the camera / eye).
     * 4. The normal vector of the \see{Shape} at the point.
     * 5. A vector indicating the reflection effect.
     * 6. Whether or not the intersection occurs inside the object (in which case, the normal is inverted to
     *    illuminate the surface properly).
     */
    class Hit final: public Intersection {
        const Tuple point;
        const Tuple under_point;
        const Tuple eyev;
        const Tuple normalv;
        const Tuple reflectv;
        const bool inside;
        const double n1;
        const double n2;

    public:
        Hit() = delete;

        Hit(const Intersection &i,
            const Tuple &point,
            const Tuple &under_point,
            const Tuple &eyev,
            const Tuple &normalv,
            const Tuple &reflectv,
            bool inside, double n1, double n2);

        Hit(const Hit&) = default;
        Hit(Hit&&) = default;

        const Tuple &getPoint() const noexcept;
        const Tuple &getUnderPoint() const noexcept;
        const Tuple &getEyeVector() const noexcept;
        const Tuple &getNormalVector() const noexcept;
        const Tuple &getReflectVector() const noexcept;
        double getN1() const noexcept;
        double getN2() const noexcept;
        bool isInside() const noexcept;

        /**
         * Calculate the Fresnel effect using the Schlick approximation.
         * This returns the reflectance, a number in [0,1] that represents the fraction of light that should be
         * reflected, given the surface information.
         */
        double schlick() const noexcept;
    };
}