/**
 * affine_transform.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>

#include "matrix.h"
#include "vec.h"

namespace raytracer {
    using Transformation = SquareMatrix<4>;

    constexpr Transformation notransform() {
        return predefined_matrices::I<double, 4>;
    }

    constexpr Transformation translation(double x, double y, double z) {
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
                    m[i][j] = const_cos(theta);
                else if (i == 1 && j == 2)
                    m[1][2] = -const_sin(theta);
                else if (i == 2 && j == 1)
                    m[2][1] = const_sin(theta);
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
                    m[i][j] = const_cos(theta);
                else if (i == 0 && j == 2)
                    m[i][j] = const_sin(theta);
                else if (i == 2 && j == 0)
                    m[i][j] = -const_sin(theta);
                else if (i == j)
                    m[i][j] = 1;
                else m[i][j] = 0;
        return Transformation{m};
    }

    constexpr Transformation rotation_z(double theta) {
        Transformation::matrix_type m{};
        for (size_t i = 0; i < 4; ++i)
            for (size_t j = 0; j < 4; ++j)
                if ((i == 0 && j == 0) || (i == 1 && j == 1))
                    m[i][j] = const_cos(theta);
                else if (i == 0 && j == 1)
                    m[i][j] = -const_sin(theta);
                else if (i == 1 && j == 0)
                    m[i][j] = const_sin(theta);
                else if (i == j)
                    m[i][j] = 1;
                else m[i][j] = 0;
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
                else m[i][j] = 0;
        return Transformation{m};
    }

    constexpr Transformation view_transform(const Tuple &from, const Tuple &to, const Tuple &upv) {
        Transformation::matrix_type m{};

        const auto forward = (to - from).normalize();
        const auto upn     = upv.normalize();
        const auto left    = forward.cross_product(upn);
        const auto true_up = left.cross_product(forward);

        // Set the rightmost column and bottom-most row to (0, 0, 0, 1).
        for (size_t i = 0; i < 3; ++i) {
            m[i][3] = 0;
            m[3][i] = 0;
        }
        m[3][3] = 1;

        // Set the rest of the matrix.
        for (size_t i = 0; i < 3; ++i) {
            m[0][i] = left[i];
            m[1][i] = true_up[i];
            m[2][i] = -forward[i];
        }

        const auto trans = translation(-from[0], -from[1], -from[2]);
        const auto transm = Transformation{m};
        const auto result = transm * trans;
        return Transformation{m} * translation(-from[0], -from[1], -from[2]);
    }
}
