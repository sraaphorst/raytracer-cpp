/**
 * Matrix.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <algorithm>
#include <array>

#include "transformers.h"
#include "vec.h"

namespace raytracer {
    using namespace transformers;

    namespace details {
        /**
         * Auxiliary functions to find the determinant. Only for square matrices.
         * Different cases for N=1, 2, and > 2.
         * Annoyingly, I can't seem to put these in Matrix, where they would be more useful due to access to cofactor.
         * The compiler complains about overloads there.
         */
        template<typename T, size_t rows, size_t cols>
        using mtxarray = std::array<std::array<T, cols>, rows>;

        template<typename T, size_t N>
        using mtxsqarray = std::array<std::array<T, N>, N>;

        template<typename T, size_t N, size_t i, size_t j>
        constexpr T array_cofactor(const mtxsqarray<T, N> &contents);

        template<typename T, size_t N, size_t i>
        struct array_determinant_helper {
            static constexpr T value(const mtxsqarray<T, N> &contents) {
                return contents[0][i] * array_cofactor<T, N, 0, i>(contents) + array_determinant_helper<T, N, i+1>::value(contents);
            }
        };

        template<typename T, size_t N>
        struct array_determinant_helper<T, N, N> {
            static constexpr T value(const mtxsqarray<T, N>&) {
                return 0;
            }
        };

        template<typename T, size_t N>
        constexpr T array_determinant(const mtxsqarray<T, N> &contents) {
            if constexpr(N == 1)      return contents[0][0];
            else if constexpr(N == 2) return contents[0][0] * contents[1][1] - contents[0][1] * contents[1][0];
            else                      return array_determinant_helper<T, N, 0>::value(contents);
        }

        /// Omit row i and column j to get a submatrix of one dimension less in rows and cols.
        template<typename T, size_t rows, size_t cols, size_t i, size_t j>
        constexpr mtxarray<T, rows-1, cols-1> array_submatrix(const mtxarray<T, rows, cols> &contents) {
            std::array<std::array<T, cols-1>, rows-1> newContents{};
            for (size_t r = 0; r < rows-1; ++r) {
                const size_t ridx = r >= i ? r + 1 : r;
                for (size_t c = 0; c < cols-1; ++c) {
                    const size_t cidx = c >= j ? c + 1 : c;
                    newContents[r][c] = contents[ridx][cidx];
                }
            }
            return newContents;
        }

        /// Calculate the minor(i,j) of a matrix, i.e. the determinant of the submatrix(i,j).
        template<typename T, size_t N, size_t i, size_t j>
        constexpr T array_minor(const mtxsqarray<T, N> &contents) {
            return array_determinant<T, N-1>(array_submatrix<T, N, N, i, j>(contents));
        }

        /// Calculate the cofactor(i,j) of a matrix, which is just (i+j)^(-1) * minor(i,j).
        template<typename T, size_t N, size_t i, size_t j>
        constexpr T array_cofactor(const mtxsqarray<T, N> &contents) {
            return ((i + j) % 2 ? -1 : 1) * array_minor<T, N, i, j>(contents);
        }

        template<typename T, size_t N, size_t r, size_t c>
        struct array_cofactor_helper {
            constexpr static void modifier(const mtxsqarray<T, N> &contents, mtxsqarray<T, N> &inv) {
                inv[r][c] = array_cofactor<T, N, r, c>(contents);
                array_cofactor_helper<T, N, r, c+1>::modifier(contents, inv);
            }
        };

        template<typename T, size_t N, size_t r>
        struct array_cofactor_helper<T, N, r, N> {
            constexpr static void modifier(const mtxsqarray<T, N> &contents, mtxsqarray<T, N> &inv) {
                if constexpr(r < N-1)
                    array_cofactor_helper<T, N, r+1, 0>::modifier(contents, inv);
            }
        };

        /// Calculate the matrix of cofactors.
        template <typename T, size_t N>
        constexpr mtxsqarray<T, N> array_cofactors(const mtxsqarray<T, N> &contents) {
            mtxsqarray<T, N> inv{};
            array_cofactor_helper<T, N, 0, 0>::modifier(contents, inv);
            return inv;
        }
    }

    template<typename T, size_t rows, size_t cols,
            typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix final {
    public:
        using type = T;
        using row_type = std::array<T, cols>;
        using col_type = std::array<T, rows>;
        using matrix_type = std::array<row_type, rows>;

    private:
        matrix_type contents;

    public:
        constexpr Matrix() noexcept {};
        constexpr Matrix(const matrix_type &contents) noexcept : contents{contents} {}
        constexpr Matrix(matrix_type&& contents) noexcept : contents{contents} {}
        constexpr Matrix(const Matrix&) noexcept = default;
        constexpr Matrix(Matrix&&) noexcept = default;
        constexpr Matrix(std::initializer_list<row_type> lst) : contents{initializer_list_to_array<row_type, rows>(lst)} {}

        ~Matrix() = default;

        constexpr Matrix &operator=(const Matrix&) noexcept = default;
        constexpr Matrix &operator=(Matrix&&) noexcept = default;

        constexpr const row_type &operator[](size_t idx) const {
            return contents[idx];
        }

        constexpr const Matrix<T, cols, rows> transpose() const {
            return transformers::details::transpose(contents);
        }

        constexpr Matrix operator+(const Matrix &other) const {
            return Matrix{contents + other.contents};
        };

        constexpr Matrix operator-(const Matrix &other) const {
            return Matrix{contents - other.contents};
        }

        template<size_t C2>
        constexpr Matrix<T, rows, C2> operator*(const Matrix<T, cols, C2> &other) const {
            std::array<std::array<T, C2>, rows> result{};
            for (size_t r = 0; r < rows; ++r) {
                for (size_t c2 = 0; c2 < C2; ++c2) {
                    result[r][c2] = 0;
                    for (size_t c = 0; c < cols; ++c)
                        result[r][c2] += contents[r][c] * other.contents[c][c2];
                }
            }
            return Matrix<T, rows, C2>(result);
        }

        constexpr Vector<T, rows> operator*(const Vector<T, cols> &v) const {
            return Vector<T, rows>{contents * v.contents};
        }

        constexpr Matrix operator*(T factor) const {
            return Matrix{factor * contents};
        }

        constexpr Matrix operator/(T denom) const {
            return Matrix{contents / denom};
        };

        /// NOTE: working with iterators can be used as constexpr here, but working with indices cannot..
        constexpr bool operator==(const Matrix &other) const {
            for (auto it1 = contents.cbegin(), it2 = other.contents.cbegin(); it1 != contents.cend(); ++it1, ++it2)
                for (auto ot1 = it1->cbegin(), ot2 = it2->cbegin(); ot1 != it1->cend(); ++ot1, ++ot2)
                    if (!ALMOST_EQUALS(*ot1, *ot2)) return false;
            return true;
        }

        constexpr bool operator!=(const Matrix &other) const {
            return !(*this == other);
        }

        static constexpr size_t row_count() {
            return rows;
        }

        static constexpr size_t column_count() {
            return cols;
        }

        static constexpr typename std::enable_if_t<are_equal_v<rows,cols>, size_t>
        size() {
            static_assert(rows == cols, "Matrix::size() only for use with square matrices");
            return rows;
        }

        /// Calculate the minor(i,j) of a matrix, i.e. the determinant of the submatrix(i,j).
        template<size_t i, size_t j>
        constexpr T minor() const {
            static_assert(rows == cols, "Matrix::minor() only for use with square matrices");
            return ::raytracer::details::array_minor<T, rows, i, j>(contents);
        }

        /// Calculate the cofactor(i,j) of a matrix, which is just (i+j)^(-1) * minor(i,j).
        template<size_t i, size_t j>
        constexpr T cofactor() const {
            static_assert(rows == cols, "Matrix::cofactor() only for use with square matrices");
            return ::raytracer::details::array_cofactor<T, rows, i, j>(contents);
        }

        constexpr T determinant() const {
            static_assert(rows == cols, "Matrix::determinant() only for use with square matrices");
            return ::raytracer::details::array_determinant<T, rows>(contents);
        }

        constexpr Matrix invert() const {
            static_assert(rows == cols, "Matrix::invert() only for use with square matrices");
            static_assert(std::is_floating_point_v<T> && std::is_signed_v<T>,
                    "Matrix::invert() only for use with signed floating point matrices");
            return Matrix{::raytracer::details::array_cofactors<T, rows>(contents)}.transpose() /
                ::raytracer::details::array_determinant<T,rows>(contents);
        }

        constexpr Matrix andThen(const Matrix &other) const {
            static_assert(rows == 4 && cols == 4, "Matrix::andThen() only for use with 4x4 matrices");
            static_assert(std::is_floating_point_v<T> && std::is_signed_v<T>,
                    "Matrix::andThen() only for use with signed floating point matrices");
            return Matrix{transformers::details::mat_mult<T, 4, 4, 4>(other.contents, contents)};
        }

        /// Omit row i and column j to get a submatrix of one dimension less in rows and cols.
        template<size_t i, size_t j>
        constexpr Matrix<T, rows-1, cols-1> submatrix() const {
            return Matrix<T, rows-1, cols-1>{{::raytracer::details::array_submatrix<T, rows, cols, i, j>(contents)}};
        }

        /// Make all matrices friends so they can access each others' contents.
        template<typename, size_t, size_t, typename>
        friend class Matrix;

        /// Multiply by factor on the left.
        friend constexpr Matrix operator*(T factor, const Matrix &m) {
            return m * factor;
        }

        /// Multiply by vector on the left.
        template<typename S, size_t m, size_t n>
        friend constexpr Vector<S,n> operator*(const Vector<S,m>&, const Matrix<S,m,n,S>&);
    };

    /// We use the property that (v * A)^T = A^t * v^t.
    template<typename S, size_t m, size_t n>
    constexpr Vector<S,n> operator*(const Vector<S,m> &v, const Matrix<S,m,n,S> &matrix) {
        return matrix.transpose() * v;
    }

    template<size_t N>
    using SquareMatrix = Matrix<double, N, N>;

    struct predefined_matrices {
        /**
         * Unlike make_array, make_uniform_matrix and make_diagonal matrix allow us to be constexpr as they don't
         * use any std::function.
         */
        template<typename T = double, size_t R = 4, size_t C = 4>
        static constexpr Matrix<T, R, C> ones = transformers::details::make_uniform_matrix<T, R, C>(1);

        /// Identity matrix, only defined as a square matrix.
        template<typename T = double, size_t N = 4>
        static constexpr Matrix<T, N, N> I = transformers::details::make_diagonal_matrix<T, N, N>(0, 1);
    };
}
