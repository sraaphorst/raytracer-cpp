/**
 * Matrix.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <algorithm>
#include <array>

#include "transformers.h"

namespace raytracer {
    using namespace details;

    template<typename T, size_t rows, size_t cols,
            typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix {
    public:
        using type = T;
        using row_type = std::array<T, cols>;
        using matrix_type = std::array<row_type, rows>;

    protected:
        matrix_type contents;

        /// Used in matrix multiplication.
        static constexpr T dot_product(const row_type &r1, const row_type &r2) noexcept {
            return details::Reducer<T, T, cols>::result(
                    [](T t1, T t2) { return t1 * t2; },
                    [](const T &t1, const T &t2) { return t1 + t2; }, 0,
                    r1, r2);
        }

    public:
        constexpr Matrix() = default;
        constexpr Matrix(const matrix_type &contents) noexcept : contents{contents} {}
        constexpr Matrix(matrix_type&& contents) noexcept : contents{contents} {}
        constexpr Matrix(const Matrix&) noexcept = default;
        constexpr Matrix(Matrix&&) noexcept = default;

        constexpr Matrix(std::initializer_list<row_type> r) {
            std::copy(r.begin(), r.end(), contents.begin());
        }

        ~Matrix() = default;

        constexpr Matrix &operator=(const Matrix&) noexcept = default;
        constexpr Matrix &operator=(Matrix&&) noexcept = default;

        constexpr const row_type &operator[](size_t idx) const {
            return contents[idx];
        }

        constexpr Matrix operator+(const Matrix &other) const {
            return Matrix{contents + other.contents};
        };

        constexpr Matrix operator-(const Matrix &other) const {
            return Matrix{contents - other.contents};
        }

        constexpr Matrix<T, rows, rows> operator*(const Matrix<T, cols, rows> &other) const {
            constexpr const Matrix &otherT = other.transpose();

            Matrix<T, rows, rows> m;
            for (int i=0; i < rows; ++i)
                for (int j=0; j < rows; ++j)
                    m.contents[i][j] = dot_product(contents[i], otherT.contents[i]);
            return m;
        }

        constexpr Matrix operator*(T factor) const {
            return Matrix{factor * contents};
        }

        constexpr Matrix operator/(T denom) const {
            return Matrix{contents / denom};
        };

        constexpr bool operator==(const Matrix &other) const {
            return equals(contents, other.contents);
        }

        constexpr bool operator!=(const Matrix &other) const {
            return !(*this == other);
        }

        constexpr Matrix<T, cols, rows> transpose() const {
            Matrix<T, cols, rows> m;
            for (auto i=0; i < rows; ++i)
                for (auto j=0; j < cols; ++j)
                    m.contents[j][i] = contents[i][j];
            return m;
        }

        static constexpr size_t row_count() {
            return rows;
        }

        static constexpr size_t column_count() {
            return cols;
        }

        friend constexpr Matrix operator*(T factor, const Matrix &m) {
            return m * factor;
        }

        static Matrix ones() {
            Matrix m;
            for (auto i=0; i < rows; ++i)
                std::fill(m.contents[i].begin(), m.contents[i].end(), 1);
            return m;
        }
    };
}
