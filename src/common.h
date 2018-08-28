/**
 * common.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "constmath.h"

#define EPSILON 1e-5
#define ALMOST_EQUALS(x, y) (absd((x) - (y)) < EPSILON)
