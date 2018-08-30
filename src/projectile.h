/**
 * projectile.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <ostream>
#include <stdexcept>

#include "tuple.h"
#include "world.h"

namespace raytracer {
    class projectile {
    private:
        const Tuple position;
        const Tuple velocity;

    public:
        constexpr projectile(const Tuple &position, const Tuple &velocity): position{position}, velocity{velocity} {
            if (!position.isPoint())
                throw std::invalid_argument("projectile requires a position for the first argument");
            if (!velocity.isVector())
                throw std::invalid_argument("projectile requires a vector for the second argument");
        }

        constexpr projectile(const projectile &other) = default;

        std::unique_ptr<projectile> tick(const world w) const {
            return std::make_unique<projectile>(position + velocity, velocity + w.gravity + w.wind);
        }

        constexpr const Tuple &getPosition() const {
            return position;
        }

        constexpr const Tuple &getVelocity() const {
            return velocity;
        }

        constexpr bool inAir() const {
            return position[Tuple::y] >= 0;
        }

        friend std::ostream &operator<<(std::ostream &os, const projectile &projectile);
    };
}


