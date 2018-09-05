/**
 * transformers.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <iomanip>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <utility>

#include "common.h"

namespace raytracer::transformers {
    namespace details {
        /// Fill a row with values. Two parameters are provided: one for elements on the diagonal, and one for all others.
        template <typename T, unsigned long C, size_t... Indices>
        constexpr std::array<T, C> make_diagonal_matrix_helper(T nondiag, T diag, size_t row, std::index_sequence<Indices...>) {
            return {{ (Indices == row ? diag : nondiag)... }};
        }

        /// Fill a matrix with values. Two parameters are provided: one for elements on the diagonal, and one for all others.
        template <typename T, unsigned long int R, unsigned long int C, size_t... Indices>
        constexpr std::array<std::array<T, C>, R> make_diagonal_matrix_helper(T nondiag, T diag, std::index_sequence<Indices...>) {
            return {{ make_diagonal_matrix_helper<T, C>(nondiag, diag, Indices, std::make_index_sequence<C>{})... }};
        }

        template<class T, unsigned long int N, size_t... Indices>
        constexpr std::array<T, N> indextransform_helper(std::function<T(int)> f, std::index_sequence<Indices...>) {
            return {{ f(Indices)... }};
        }

        template<class R, class T, unsigned long int N, size_t... Indices>
        constexpr std::array<R, N> unitransform_helper(std::function<R(const T&)> f, const std::array<T, N> &a,
                                                       std::index_sequence<Indices...>) {
            return {{ f(a[Indices])... }};
        }

        template<class R, class T, unsigned long int N, size_t... Indices>
        constexpr std::array<T, N> bitransform_helper(std::function<R(const T&, const T&)> f, const std::array<T, N> &a1, const std::array<T, N> &a2,
                                                      std::index_sequence<Indices...>) {
            return {{ f(a1[Indices], a2[Indices])... }};
        }
    }

    /// Checked for constexpr.
    template<typename T, size_t N>
    constexpr T dot_product(const std::array<T,N> &a1, const std::array<T,N> &a2) {
        T t{};
        for (int i=0; i < N; ++i)
            t += a1[i] * a2[i];
        return t;
    }

    /// Checked for constexpr.
    template<typename T, size_t R, size_t C, size_t... Indices>
    constexpr std::array<T, R> transpose_row_helper(const std::array<std::array<T, C>, R> &m, size_t col, std::index_sequence<Indices...>) {
        return {{m[Indices][col]...}};
    }
    template<typename T, size_t R, size_t C, size_t... Indices>
    constexpr std::array<std::array<T, R>, C> transpose_helper(const std::array<std::array<T, C>, R> &m, std::index_sequence<Indices...>) {
        return {{transpose_row_helper<T,R,C>(m, Indices, std::make_index_sequence<R>{})...}};
    }
    template<typename T, size_t R, size_t C>
    constexpr std::array<std::array<T, R>, C> transpose(const std::array<std::array<T, C>, R> &m) {
        return transpose_helper<T,R,C>(m, std::make_index_sequence<C>{});
    }

    /// TODO: REWRITE
    /// Create simple 2-D arrays where the value at pos (i,i) is one value, and the value at position (i,j), i != j, is another.
    template <typename T, size_t R, size_t C>
    constexpr std::array<std::array<T, C>, R> make_diagonal_matrix(T nondiag, T diag) {
        return details::make_diagonal_matrix_helper<T, R, C>(nondiag, diag, std::make_index_sequence<R>{});
    }

    template <typename T, size_t R, size_t C>
    constexpr std::array<std::array<T, C>, R> make_uniform_matrix(T fill) {
        return make_diagonal_matrix<T,R,C>(fill, fill);
    }

    /// Execute a transformation on a range of indices.
    template<class T, unsigned long int N>
    constexpr std::array<T, N> indextransform(std::function<T(int)> f) {
        return details::indextransform_helper<T, N>(f, std::make_index_sequence<N>{});
    }

    /// Execute a transformation on an array for each index.
    template<class R, class T, unsigned long int N>
    constexpr std::array<R, N> unitransform(std::function<R(const T&)> f, const std::array<T, N> &a) {
        return details::unitransform_helper(f, a, std::make_index_sequence<N>{});
    }

    /// Execute a transformation on a pair of arrays for each index.
    template<class R, class T, unsigned long int N>
    constexpr std::array<T, N> bitransform(std::function<R(const T&, const T&)> f, const std::array<T, N> &a1, const std::array<T, N> &a2) {
        return details::bitransform_helper(f, a1, a2, std::make_index_sequence<N>{});
    }

    /// Checked for constexpr.
    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> vector_opadd_helper(const std::array<T,N> &t1, const std::array<T,N> &t2, std::index_sequence<Indices...>) {
        return {{t1[Indices] + t2[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator+(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return vector_opadd_helper<T,N>(t1, t2, std::make_index_sequence<N>{});
    }
    template<typename T, size_t R, size_t C, size_t... Indices>
    constexpr std::array<std::array<T, C>, R> matrix_opadd_helper(const std::array<std::array<T, C>, R> &m1, const std::array<std::array<T, C>, R> &m2, std::index_sequence<Indices...>) {
        return {{m1[Indices] + m2[Indices]...}};
    }
    template<typename T, size_t R, size_t C>
    constexpr std::array<std::array<T, C>, R> operator+(const std::array<std::array<T, C>, R> &m1, const std::array<std::array<T, C>, R> &m2) {
        return matrix_opadd_helper<T,R,C>(m1, m2, std::make_index_sequence<R>{});
    }

    /// Checked for constexpr.
    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> vector_opdiff_helper(const std::array<T,N> &t1, const std::array<T,N> &t2, std::index_sequence<Indices...>) {
        return {{t1[Indices] - t2[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator-(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        //return bitransform<T,T,N>([](const T &a, const T &b) { return a - b; }, t1, t2);
        return vector_opdiff_helper<T,N>(t1, t2, std::make_index_sequence<N>{});
    }
    template<typename T, size_t R, size_t C, size_t... Indices>
    constexpr std::array<std::array<T, C>, R> matrix_opdiff_helper(const std::array<std::array<T, C>, R> &m1, const std::array<std::array<T, C>, R> &m2, std::index_sequence<Indices...>) {
        return {{m1[Indices] - m2[Indices]...}};
    }
    template<typename T, size_t R, size_t C>
    constexpr std::array<std::array<T, C>, R> operator-(const std::array<std::array<T, C>, R> &m1, const std::array<std::array<T, C>, R> &m2) {
        return matrix_opdiff_helper<T,R,C>(m1, m2, std::make_index_sequence<R>{});
    }

    /// Checked for constexpr.
    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> vector_opmult_helper(const std::array<T,N> &t1, const std::array<T,N> &t2, std::index_sequence<Indices...>) {
        return {{t1[Indices] * t2[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator*(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return vector_opmult_helper<T,N>(t1, t2, std::make_index_sequence<N>{});
    }

    /// NOT CHECKED. C1 must be R1 so omit.
    template<typename T, size_t R, size_t C1, size_t C2, size_t... Indices>
    constexpr std::array<T, C2> matrix_row_mult_helper(const std::array<std::array<T, C1>, R> &m1, const std::array<std::array<T, C1>, C2> &m2, size_t row, std::index_sequence<Indices...>) {
        return {{ dot_product<T,C1>(m1[row], m2[Indices])...}};
    }
    template<typename T, size_t R, size_t C1, size_t C2, size_t... Indices>
    constexpr std::array<std::array<T, C2>, R> matrix_mult_helper(const std::array<std::array<T, C1>, R> &m1, const std::array<std::array<T, C1>, C2> &m2t, std::index_sequence<Indices...>) {
        return {{ matrix_row_mult_helper<T,R,C1,C2>(m1, m2t, Indices, std::make_index_sequence<C2>{})...}};
    }
    template<typename T, size_t R, size_t C1, size_t C2>
    constexpr std::array<std::array<T, C2>, R> mat_mult(const std::array<std::array<T, C1>, R> &m1, const std::array<std::array<T, C2>, C1> &m2) {
        return matrix_mult_helper<T, R, C1, C2>(m1, transpose<T, C1, C2>(m2), std::make_index_sequence<R>{});
    }

    /// Checked for constexpr.
    template<typename F, typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N>
    scalar_opmult_helper(F f, const std::array<T,N> &t, std::index_sequence<Indices...>) {
        return {{f * t[Indices]...}};
    }
    template<typename F, typename T, size_t N>
    constexpr typename std::enable_if_t<std::is_arithmetic_v<F>, std::array<T,N>>
    operator*(F f, const std::array<T,N> &t) {
        return scalar_opmult_helper<F,T,N>(f, t, std::make_index_sequence<N>{});
    }
    template<typename F, typename T, size_t R, size_t C, size_t... Indices>
    constexpr typename std::enable_if_t<std::is_arithmetic_v<F>, const std::array<std::array<T, C>, R>>
    matrix_opmult_helper(F f, const std::array<std::array<T, C>, R> &m1, std::index_sequence<Indices...>) {
        return {{ f * m1[Indices]... }};
    }
    template<typename F, typename T, size_t R, size_t C>
    constexpr typename std::enable_if_t<std::is_arithmetic_v<F>, const std::array<std::array<T, C>, R>>
    operator*(F f, const std::array<std::array<T, C>, R> &m) {
        return matrix_opmult_helper<F,T,R,C>(f, m, std::make_index_sequence<R>{});
    }

    /// UNCHECKED
    template<typename T, size_t R, size_t C, size_t... Indices>
    constexpr std::array<T, R> matrix_vector_opmult_helper(const std::array<std::array<T, C>, R> &m, const std::array<T, C> &v, std::index_sequence<Indices...>) {
        return {{ dot_product<T,C>(m[Indices], v)...}};
    }
    template<typename T, size_t R, size_t C>
    constexpr std::array<T, R> operator*(const std::array<std::array<T, C>, R> &m, const std::array<T, C> &v) {
        return matrix_vector_opmult_helper<T,R,C>(m, v, std::make_index_sequence<R>{});
    }

    /// Checked for constexpr: ONLY WORKS IF T2 HAS NO ZEROES.
    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N>
    vector_opdiv_helper(const std::array<T,N> &t1, const std::array<T,N> &t2, std::index_sequence<Indices...>) {
        return {{(t1[Indices] / t2[Indices])...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator/(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return vector_opdiv_helper<T,N>(t1, t2, std::make_index_sequence<N>{});
    }

    /// Checked for constexpr.
    template<typename F, typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> scalar_opdiv_helper(const std::array<T,N> &t, F f, std::index_sequence<Indices...>) {
        return {{t[Indices] / f...}};
    }
    template<typename F, typename T, size_t N>
    constexpr typename std::enable_if_t<std::is_arithmetic_v<F>, std::array<T,N>>
    operator/(const std::array<T,N> &t, F f) {
        return scalar_opdiv_helper<F,T,N>(t, f, std::make_index_sequence<N>{});
    }

    /// Checked for constexpr.
    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> vector_neg_helper(const std::array<T,N> &t, std::index_sequence<Indices...>) {
        return {{-t[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator-(const std::array<T,N> &t) {
        return vector_neg_helper<T,N>(t, std::make_index_sequence<N>{});
    }

    /// Create an array where the elements are determined by a supplied function invoked on index.
    template<class T, size_t N>
    static constexpr std::array<T, N> make_array(const std::function<T(int)> f) {
        return indextransform<T,N>(f);
    }

    /// Test at compile time if two sizes are equal.
    template<size_t m, size_t n>
    struct are_equal final: std::false_type {};

    template<size_t m>
    struct are_equal<m,m> final: std::true_type {};

    template<size_t m, size_t n>
    inline constexpr bool are_equal_v = are_equal<m,n>::value;


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
            return array_cofactor<T, N , 0, i>(contents) + array_determinant_helper<T, N, i+1>::value(contents);
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
        return ((i + j) % 2 ? -1 : 1) * contents[i][j] * array_minor<T, N, i, j>(contents);
    }


    /// Checked for constexpr.
    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> initializer_list_to_array_helper(const std::initializer_list<T> lst, std::index_sequence<Indices...>) {
        return {{lst.begin()[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> initializer_list_to_array(const std::initializer_list<T> lst) {
        return initializer_list_to_array_helper<T,N>(lst, std::make_index_sequence<N>{});
    }
}
