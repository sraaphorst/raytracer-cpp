/**
 * pattern.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "pattern.h"

namespace raytracer {
    bool Pattern::operator==(const Pattern &other) const noexcept {
        return typeid(*this) == typeid(other) && doCompare(other);
    }

    bool Pattern::operator!=(const raytracer::Pattern &other) const noexcept {
        return !(*this == other);
    }

    bool Pattern::doCompare(const Pattern &other) const noexcept {
        return true;
    }
}