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
    namespace transformer_details {
//        /** Make diagonal matrix helper **/
//        template <typename T, unsigned long C, size_t... Indices>
//        constexpr std::array<T, C> make_diagonal_matrix_helper(T nondiag, T diag, size_t row, std::index_sequence<Indices...>) {
//            return {{ (Indices == row ? diag : nondiag)... }};
//        }

        /** Transpose matrix helpers **/
        template<typename T, size_t R, size_t C, size_t... Indices>
        constexpr std::array<T, R> transpose_row_helper(const std::array<std::array<T, C>, R> &m, size_t col, std::index_sequence<Indices...>) {
            return {{m[Indices][col]...}};
        }
        template<typename T, size_t R, size_t C, size_t... Indices>
        constexpr std::array<std::array<T, R>, C> transpose_helper(const std::array<std::array<T, C>, R> &m, std::index_sequence<Indices...>) {
            return {{transpose_row_helper<T,R,C>(m, Indices, std::make_index_sequence<R>{})...}};
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

    template<typename T, size_t R, size_t C>
    constexpr std::array<std::array<T, R>, C> transpose(const std::array<std::array<T, C>, R> &m) {
        return transformer_details::transpose_helper<T,R,C>(m, std::make_index_sequence<C>{});
    }

    /// TODO: REWRITE
    /// Create simple 2-D arrays where the value at pos (i,i) is one value, and the value at position (i,j), i != j, is another.
//    template <typename T, size_t R, size_t C>
//    constexpr std::array<std::array<T, C>, R> make_diagonal_matrix(T nondiag, T diag) {
//        return details::make_diagonal_matrix_helper<T, R, C>(nondiag, diag, std::make_index_sequence<R>{});
//    }

    template <typename T, size_t R, size_t C>
    constexpr std::array<std::array<T, C>, R> make_diagonal_matrix(T nondiag, T diag) {
        std::array<std::array<T, C>, R> arr{};
        for (size_t r = 0; r < R; ++r)
            for (size_t c = 0; c < C; ++c)
                arr[r][c] = (r == c) ? diag : nondiag;
        return arr;
    }

    template <typename T, size_t R, size_t C>
    constexpr std::array<std::array<T, C>, R> make_uniform_matrix(T fill) {
        return make_diagonal_matrix<T,R,C>(fill, fill);
    }

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

    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> vector_opdiff_helper(const std::array<T,N> &t1, const std::array<T,N> &t2, std::index_sequence<Indices...>) {
        return {{t1[Indices] - t2[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator-(const std::array<T,N> &t1, const std::array<T,N> &t2) {
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

    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> vector_opmult_helper(const std::array<T,N> &t1, const std::array<T,N> &t2, std::index_sequence<Indices...>) {
        return {{t1[Indices] * t2[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator*(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return vector_opmult_helper<T,N>(t1, t2, std::make_index_sequence<N>{});
    }

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

    template<typename T, size_t R, size_t C, size_t... Indices>
    constexpr std::array<T, R> matrix_vector_opmult_helper(const std::array<std::array<T, C>, R> &m, const std::array<T, C> &v, std::index_sequence<Indices...>) {
        return {{ dot_product<T,C>(m[Indices], v)...}};
    }
    template<typename T, size_t R, size_t C>
    constexpr std::array<T, R> operator*(const std::array<std::array<T, C>, R> &m, const std::array<T, C> &v) {
        return matrix_vector_opmult_helper<T,R,C>(m, v, std::make_index_sequence<R>{});
    }

    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N>
    vector_opdiv_helper(const std::array<T,N> &t1, const std::array<T,N> &t2, std::index_sequence<Indices...>) {
        return {{(t1[Indices] / t2[Indices])...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator/(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return vector_opdiv_helper<T,N>(t1, t2, std::make_index_sequence<N>{});
    }

    template<typename F, typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> scalar_opdiv_helper(const std::array<T,N> &t, F f, std::index_sequence<Indices...>) {
        return {{t[Indices] / f...}};
    }
    template<typename F, typename T, size_t N>
    constexpr typename std::enable_if_t<std::is_arithmetic_v<F>, std::array<T,N>>
    operator/(const std::array<T,N> &t, F f) {
        return scalar_opdiv_helper<F,T,N>(t, f, std::make_index_sequence<N>{});
    }

    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> vector_neg_helper(const std::array<T,N> &t, std::index_sequence<Indices...>) {
        return {{-t[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> operator-(const std::array<T,N> &t) {
        return vector_neg_helper<T,N>(t, std::make_index_sequence<N>{});
    }

    template<size_t m, size_t n>
    struct are_equal final: std::false_type {};

    template<size_t m>
    struct are_equal<m,m> final: std::true_type {};

    template<size_t m, size_t n>
    inline constexpr bool are_equal_v = are_equal<m,n>::value;

    template<typename T, size_t N, size_t... Indices>
    constexpr std::array<T,N> initializer_list_to_array_helper(const std::initializer_list<T> lst, std::index_sequence<Indices...>) {
        return {{lst.begin()[Indices]...}};
    }
    template<typename T, size_t N>
    constexpr std::array<T,N> initializer_list_to_array(const std::initializer_list<T> lst) {
        return initializer_list_to_array_helper<T,N>(lst, std::make_index_sequence<N>{});
    }
}
