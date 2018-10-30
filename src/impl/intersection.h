/**
 * intersection.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <optional>
#include <vector>

#include "shapes/shape.h"
#include "transformers.h"

namespace raytracer::shapes {
    class Shape;
}

namespace raytracer::impl {
    class Hit;
    class Ray;

    /**
     * Defines the concept of an intersection, which is a \see{Ray} striking a \see{Shape}.
     * A \see{Ray} that strikes a \see{Shape} is called a \see{Hit}, which contains additional information about
     * the intersection.
     */
    class Intersection {
    private:
        double t;

        // Intersections should not be allowed to modify their shapes.
        std::shared_ptr<const shapes::Shape> o;

    public:
        Intersection() = delete;

        /// Define an Intersection at time t with a \see{Shape}.
        Intersection(double t, const std::shared_ptr<const shapes::Shape> &o) noexcept;

        Intersection(const Intersection&) noexcept = default;
        Intersection(Intersection&&) noexcept = default;
        Intersection &operator=(const Intersection &other) noexcept = default;

        bool operator==(const Intersection &other) const noexcept;
        bool operator!=(const Intersection &other) const noexcept;

        double getT() const noexcept;
        const std::shared_ptr<const shapes::Shape> &getObject() const noexcept;

        /**
         * Static hit method. This takes a list of intersections, and determines the intersection that is visible,
         * i.e. the intersection with the lowest positive time value.
         *
         * @param xs the list of intersections
         * @return the intersection, or nullopt if no such intersection exists
         */
        static const std::optional<const Intersection> hit(const std::vector<Intersection> &xs) noexcept;

        /**
         * Given an optional intersection, create a fully-fledged \see{Hit} from it.
         * @param hit an optional intersection
         * @param ray the ray that generated the intersection
         * @param xs the list of all intersections for the ray
         * @return a Hit if the intersection is defined, and nullopt otherwise
         */
        static const std::optional<const Hit> prepareHit(const std::optional<const Intersection> &hit,
                                                         const Ray &ray,
                                                         const std::vector<Intersection> &xs) noexcept;


        /**
         * Given an intersection, create a fully-fledged \see{Hit} from it.
         * @param hit the intersection
         * @param ray the ray that generated the intersection
         * @param xs the list of all intersections for the ray
         * @return a fully defined Hit
         */
        static const Hit prepareHit(const Intersection &hit,
                                    const Ray &ray,
                                    const std::vector<Intersection> &xs) noexcept;

    protected:
        /// Any additional implementation for equality comparison in subclasses should be implemented here.
        virtual bool doCompare(const Intersection &other) const noexcept;
    };
}
