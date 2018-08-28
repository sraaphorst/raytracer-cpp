/**
 * world.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <stdexcept>
#include <ostream>

#include "tup.h"

namespace raytracer {
    class projectile;

    class world {
    private:
        const tup gravity;
        const tup wind;

    public:
        constexpr world(tup gravity, tup wind): gravity{std::move(gravity)}, wind{std::move(wind)} {
            if (!gravity.isVector())
                throw std::invalid_argument("world requires a vector for the first argument");
            if (!wind.isVector())
                throw std::invalid_argument("world requires a vector for the second argument");
        }

        friend std::ostream &operator<<(std::ostream &os, const world &world);
        friend class projectile;
    };
}

