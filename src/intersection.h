/**
 * intersection.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <algorithm>
#include <initializer_list>
#include <optional>
#include <vector>

#include "shape.h"
#include "transformers.h"

namespace raytracer {
    using namespace transformers;

    class Intersection final {
    private:
        const double t;
        const Shape &o;

    public:
        Intersection() = delete;

        // We can't use an initializer_list due to mixed types, and a variadic template constructor is unnecessary.
        // This first constructor should be able to handle all the cases.
        Intersection(const double t, const Shape &o): t{t}, o{o} {}
        Intersection(const Intersection&) = default;
        Intersection(Intersection&&) = default;

        ~Intersection() = default;

        bool operator==(const Intersection &other) const {
            return t == other.t && o == other.o;
        }

        double getT() const noexcept {
            return t;
        }

        const Shape &getObject() const noexcept {
            return o;
        }

        // Static aggregator method.
        static std::vector<Intersection> aggregate(std::initializer_list<Intersection> lst) {
            return std::vector<Intersection>{lst};
        }

        // Static hit method: the intersection that is visible. This is always the lowest non-negative intersection.
        static const std::optional<const Intersection> hit(const std::vector<Intersection> &ints) {
            if (ints.empty())
                return {};

            Intersection const *curr = nullptr;
            for (const Intersection &i: ints) {
                if (i.getT() > 0 && (curr == nullptr || i.getT() < curr->getT()))
                    curr = &i;
            }
            if (curr == nullptr || curr->getT() < 0)
                return {};
            else
                return {*curr};
//            const auto iter = std::min(std::cbegin(ints), std::cend(ints), [](const Intersection &i1, const Intersection &i2) {
//                if (i1.getT() < 0) return i2;
//                else if (i2.getT() < 0) return i1;
//                else if (i1.getT() < i2.getT()) return i1;
//                else return i2;
//            });
//
//            if (iter == std::cend(ints) || iter->getT() < 0)
//                return {};
//            else
//                return {*iter};
        }
    };
}