/**
 * worldconditions.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <stdexcept>
#include <ostream>

#include "vec.h"

namespace raytracer {
    class Projectile;

    class WorldConditions {
    private:
        const Tuple gravity;
        const Tuple wind;

    public:
        constexpr WorldConditions(Tuple gravity, Tuple wind): gravity{gravity}, wind{wind} {
            if (!gravity.isVector())
                throw std::invalid_argument("world requires a vector for the first argument");
            if (!wind.isVector())
                throw std::invalid_argument("world requires a vector for the second argument");
        }

        friend std::ostream &operator<<(std::ostream &os, const WorldConditions &world);
        friend class Projectile;
    };
}

