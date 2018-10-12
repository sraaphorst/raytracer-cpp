/**
 * projectile.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cassert>
#include <memory>
#include <ostream>
#include <stdexcept>

#include "vector.h"
#include "worldconditions.h"

namespace raytracer {
    class Projectile {
    private:
        Tuple position;
        Tuple velocity;

    public:
        constexpr Projectile(const Tuple &position, const Tuple &velocity): position{position}, velocity{velocity} {
            if (!position.isPoint())
                throw std::invalid_argument("projectile requires a position for the first argument");
            if (!velocity.isVector())
                throw std::invalid_argument("projectile requires a vector for the second argument");
        }

        constexpr Projectile(const Projectile &other) = default;

        Projectile tick(const WorldConditions w) const {
//            position = position + velocity;
//            velocity = velocity + w.gravity + w.wind;
return Projectile{position + velocity, velocity + w.gravity + w.wind};
        }

        constexpr const Tuple &getPosition() const {
            return position;
        }

        constexpr const Tuple &getVelocity() const {
            return velocity;
        }

        constexpr bool inAir() const {
            return position[tuple_constants::y] >= 0;
        }

        friend std::ostream &operator<<(std::ostream &os, const Projectile &projectile);
    };
}


