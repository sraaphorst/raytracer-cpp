/**
 * common.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cmath>

#define EPSILON 1e-5
#define ALMOST_EQUALS(x, y) (std::abs((x) - (y)) < EPSILON)
