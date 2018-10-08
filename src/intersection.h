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

#include "shape.h"
#include "transformers.h"

namespace raytracer {
    using namespace transformers;

    class Intersection final {
    private:
        double t;
        std::shared_ptr<const Shape> o;

    public:
        Intersection() = delete;

        // We can't use an initializer_list due to mixed types, and a variadic template constructor is unnecessary.
        // This first constructor should be able to handle all the cases.
        //Intersection(const double t, const Shape &o): t{t}, o{std::make_shared(o)} {}
        Intersection(const double t, const std::shared_ptr<const Shape> o): t{t}, o{o} {}
        Intersection(const Intersection&) = default;
        Intersection(Intersection&&) = default;

        Intersection &operator=(const Intersection &other) noexcept = default;

        constexpr bool operator==(const Intersection &other) const {
            return t == other.t && o == other.o;
        }

        constexpr double getT() const noexcept {
            return t;
        }

        const Shape &getObject() const noexcept {
            return *o;
        }

        // Static aggregator method.
        static std::vector<Intersection> aggregate(std::initializer_list<Intersection> lst) {
            return std::vector<Intersection>{lst};
        }

        // Static hit method: the intersection that is visible. This is always the lowest non-negative intersection.
        static const std::optional<const Intersection> hit(const std::vector<Intersection> &ints);

        static const std::optional<const Intersection> prepare_hit(const std::optional<const Intersection> &hit,
                const Ray &ray);
    };
}