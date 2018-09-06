/**
 * affine_transform.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>

#include "matrix.h"
#include "vector.h"

namespace raytracer {
    using Transformation = SquareMatrix<4>;

    constexpr Transformation translate(double x, double y, double z) {
        std::array<double, 3> point{x, y, z};

        Transformation::matrix_type m{};
        for (size_t i = 0; i < 4; ++i)
            for (size_t j = 0; j < 4; ++j)
                if (i == j)
                    m[i][j] = 1;
                else if (j == 3)
                    m[i][3] = point[i];
                else
                    m[i][j] = 0;
        return Transformation{m};
    }

    constexpr Transformation translate(const std::array<double, 3> &t) {
        return translate(t[0], t[1], t[2]);
    }

    constexpr Transformation translate(const Tuple &t) {
        return translate(t[0], t[1], t[2]);
    }

    constexpr Transformation scale(double x, double y, double z) {
        Transformation::matrix_type m{};
        for (size_t i = 0; i < 4; ++i)
            for (size_t j = 0; j < 4; ++j)
                m[i][j] = 0;
        m[0][0] = x;
        m[1][1] = y;
        m[2][2] = z;
        m[3][3] = 1;
        return Transformation{m};
    }

    constexpr Transformation rotation_x(double theta) {
        Transformation::matrix_type m{};
        for (size_t i = 0; i < 4; ++i)
            for (size_t j = 0; j < 4; ++j)
                if ((i == 1 && j == 1) || (i == 2 && j == 2))
                    m[i][j] = cosc(theta);
                else if (i == 1 && j == 2)
                    m[1][2] = -sinc(theta);
                else if (i == 2 && j == 1)
                    m[2][1] = sinc(theta);
                else if (i == j)
                    m[i][j] = 1;
                else
                    m[i][j] = 0;
        return Transformation{m};
    }

    constexpr Transformation rotation_y(double theta) {
        Transformation::matrix_type m{};
        for (size_t i = 0; i < 4; ++i)
            for (size_t j = 0; j < 4; ++j)
                if ((i == 0 && j == 0) || (i == 2 && j == 2))
                    m[i][j] = cosc(theta);
                else if (i == 0 && j == 2)
                    m[i][j] = sinc(theta);
                else if (i == 2 && j == 0)
                    m[i][j] = -sinc(theta);
                else if (i == j)
                    m[i][j] = 1;
                else m[i][j] == 0;
        return Transformation{m};
    }

    constexpr Transformation rotation_z(double theta) {
        Transformation::matrix_type m{};
        for (size_t i = 0; i < 4; ++i)
            for (size_t j = 0; j < 4; ++j)
                if ((i == 0 && j == 0) || (i == 1 && j == 1))
                    m[i][j] = cosc(theta);
                else if (i == 0 && j == 1)
                    m[i][j] = -sinc(theta);
                else if (i == 1 && j == 0)
                    m[i][j] = sinc(theta);
                else if (i == j)
                    m[i][j] = 1;
                else m[i][j] == 0;
        return Transformation{m};
    }

    constexpr Transformation skew(double x_y, double x_z, double y_x, double y_z, double z_x, double z_y) {
        Transformation::matrix_type m{};
        for (size_t i = 0; i < 4; ++i)
            for (size_t j = 0; j < 4; ++j)
                if (i == 0 && j == 1)
                    m[i][j] = x_y;
                else if (i == 0 && j == 2)
                    m[i][j] = x_z;
                else if (i == 1 && j == 0)
                    m[i][j] = y_x;
                else if (i == 1 && j == 2)
                    m[i][j] = y_z;
                else if (i == 2 && j == 0)
                    m[i][j] = z_x;
                else if (i == 2 && j == 1)
                    m[i][j] = z_y;
                else if (i == j)
                    m[i][j] = 1;
                else m[i][j] == 0;
        return Transformation{m};
    }
}
