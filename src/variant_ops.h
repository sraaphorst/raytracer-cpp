/**
 * variant_ops.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * This file comprises operations that allow processing of variants and arrays of variants.
 * This includes:
 * 1. Making std::variant a monoid.
 * 2. Allowing addition of a value of type T to an array of variant, and having the variant types, if necessary, grow to
 *    include T. This can be either a prepend or append operation.
 * 3. Invoking an operation across an array of variant. This assumes that the types in the array all have a method in
 *    common, and is used to simulate compile-time polymorphism.
 */

#pragma once

#include <array>
#include <type_traits>
#include <variant>

#include "meta.h"

namespace raytracer::variant_ops {
    /**
     * Variant forms a monoid with zero element std::monostate.
     * The operation is concatenate and we treat types not wrapped in a variant as if they were for convenience.
     */
    struct Variant_Monoid {
        using zero = std::monostate;

        template <typename T, typename... Args>
        struct concatenate;

        template <typename... Args0, typename... Args1>
        struct concatenate<std::variant<Args0...>, std::variant<Args1...>> {
            using type = std::variant<Args0..., Args1...>;
        };

        // Convenience method to concatenate types without having to wrap them into a variant first.
        template<typename... Args0, typename... Args1>
        struct concatenate<std::variant<Args0...>, Args1...> {
            using type = std::variant<Args0..., Args1...>;
        };
    };

    namespace details {
        /**
         * Concatenate-prepend one argument to a variant, provided it isn't already in the variant types.
         */
        template<typename T, typename... Args0>
        struct concat1_prepend;

        template<typename ArgNew, typename... Args>
        struct concat1_prepend<ArgNew, std::variant<Args...>> {
            using type = std::conditional_t<
                    meta::contains<ArgNew, Args...>::value,
                    std::variant<Args...>,
                    std::variant<ArgNew, Args...>>;
        };

        /**
         * Concatenate-append one argument to a variant, provided it isn't already in the variant types.
         */
        template<typename T, typename... Args0>
        struct concat1_append;

        template<typename ArgNew, typename... Args>
        struct concat1_append<ArgNew, std::variant<Args...>> {
            using type = std::conditional_t<
                    meta::contains<ArgNew, Args...>::value,
                    std::variant<Args...>,
                    std::variant<Args..., ArgNew>>;
        };


        template<typename R, typename V, size_t N, typename Function, size_t... Indices>
        constexpr std::array<R, N>
        va_map_aux(const std::array<V, N> &v, Function f, std::index_sequence<Indices...>) noexcept {
            return {{std::visit(f, v[Indices])...}};
        }

        template<size_t N, typename T, typename S, size_t... Indices>
        constexpr std::array<T, N+1>
        va_prepend_existing_type_element_aux(const S &s, const std::array<T, N> &a, std::index_sequence<Indices...>) noexcept {
            return {{T{s}, a[Indices]...}};
        }

        template<size_t N, typename T, typename S, size_t... Indices>
        constexpr std::array<T, N + 1>
        va_append_existing_type_element_aux(const std::array<T, N> &a, const S &s, std::index_sequence<Indices...>) noexcept {
            return {{a[Indices]..., T{s}}};
        }

        template<size_t N, typename T, typename S, size_t... Indices>
        constexpr std::array<typename Variant_Monoid::concatenate<T, S>::type, N + 1>
        va_prepend_element_with_type_aux(const S &s, const std::array<T, N> &a, std::index_sequence<Indices...>) noexcept {
            return {{s, std::visit([](auto &&t) -> typename Variant_Monoid::concatenate<T, S>::type { return t; }, a[Indices])...}};
        }

        template<size_t N, typename T, typename S, size_t... Indices>
        constexpr std::array<typename Variant_Monoid::concatenate<T, S>::type, N + 1>
        va_append_element_with_type_aux(const std::array<T, N> &a, const S &s, std::index_sequence<Indices...>) noexcept {
            return {{std::visit([](auto &&t) -> typename Variant_Monoid::concatenate<T, S>::type { return t; }, a[Indices])..., s}};
        }

        template<size_t N, typename T, typename S, size_t... Indices>
        constexpr std::array<typename details::concat1_prepend<S, T>::type, N + 1>
        va_prepend_element_aux(const S &s, const std::array<T, N> &a, std::index_sequence<Indices...>) noexcept {
            return {{s, std::visit([](auto &&t) -> typename details::concat1_prepend<S, T>::type { return t; }, a[Indices])...}};
        }

        template<size_t N, typename T, typename S, size_t... Indices>
        constexpr std::array<typename details::concat1_append<S, T>::type, N + 1>
        va_append_element_aux(const std::array<T, N> &a, const S &s, std::index_sequence<Indices...>) noexcept {
            return {{std::visit([](auto &&t) -> typename details::concat1_append<S, T>::type { return t; }, a[Indices])..., s}};
        }
    }

    /**
     * Map a pseudopolymorphic function across an array of variant.
     * For example, say we have three structs unrelated by any concept of inheritance:
     *     struct S1 { constexpr int val() const { return 1; }};
     *     struct S2 { constexpr int val() const { return 2; }};
     *     struct S3 { constexpr int val() const { return 3; }};
     * And:
     *     constexpr std::array<std::variant<S1, S2, S3>, 4> va = {{S1{}, S2{}, S3{}, S2{}}};
     * Then the result of calling:
     *     constexpr auto result = va_map(va, [](auto &&a){return a.val();});
     * would be:
     *     constexpr std::array<int, 4>{{1, 2, 3, 2}};
     */
    template<typename R, typename V, size_t N, typename Function>
    constexpr std::array<R, N> va_map(const std::array<V, N> &v, Function f) noexcept {
        return details::va_map_aux<R, V, N, Function>(v, f, std::make_index_sequence<N>{});
    }

    /**
     * Allow prepending an element to an array of variant, provided the element type is already in the variant.
     * For example:
     *    If T = std::variant<int, std::string>
     *    and my_array is an std::array<T, 2> with contents {T{3}, T{"hello"}}
     *    then va_prepend_existing_type_element("world", my_array)
     *    would return an std::array<T, 3> with contents {T{"world"}, T{3}, T{"hello"}}.
     */
    template<size_t N, typename T, typename S>
    constexpr std::array<T, N + 1> va_prepend_existing_type_element(const S &s, const std::array<T, N> &a) noexcept {
        return details::va_prepend_existing_type_element_aux(s, a, std::make_index_sequence<N>{});
    }

    /**
     * Allow appending an element to an array of variant, provided the element type is in the variant.
     * For example:
     *    If T = std::variant<int, std::string_view>
     *    and my_array is an std::array<T, 2> with contents {T{3}, T{"hello"}}
     *    then va_append_existing_type_element(my_array, "world")
     *    would return an std::array<T, 3> with contents {T{3}, T{"hello"}, T{"world"}}.
     */
    template<size_t N, typename T, typename S>
    constexpr std::array<T, N + 1> va_append_existing_type_element(const std::array<T, N> &a, const S &s) noexcept {
        return details::va_append_existing_type_element_aux(a, s, std::make_index_sequence<N>{});
    }

    /**
     * Allow prepending an element to an array of variant, provided the element type is not in the variant.
     * For example:
     *    If T = std::variant<int>
     *    and my_array is an std::array<T, 2> with contents {T{3}, T{4}}
     *    then va_prepend_element_with_type(3.114159, my_array)
     *    would result in a T' = std::variant<double, int>
     *    and would return an std::array<T', 2> with contents {T'{3.14159}, T'{3}, T'{4}}.
     */
    template<size_t N, typename T, typename S>
    constexpr std::array<typename Variant_Monoid::concatenate<T, S>::type, N + 1>
    va_prepend_element_with_type(const S &s, const std::array<T, N> &a) noexcept {
        return details::va_prepend_element_with_type_aux(s, a, std::make_index_sequence<N>{});
    }

    /**
     * Allow appending an element to an array of variant, provided the element type is not in the variant.
     * For example:
     *    If T = std::variant<int>
     *    and my_array is an std::array<T, 2> with contents {T{3}, T{4}}
     *    then va_prepend_element_with_type(my_array, 3.114159)
     *    would result in a T' = std::variant<int, double>
     *    and would return an std::array<T', 2> with contents {T'{3}, T'{4}, T'{3.14159}}.
     */
    template<size_t N, typename T, typename S>
    constexpr std::array<typename Variant_Monoid::concatenate<T, S>::type, N + 1>
    va_append_element_with_type(const std::array<T, N> &a, const S &s) noexcept {
        return details::va_append_element_with_type_aux(a, s, std::make_index_sequence<N>{});
    }

    /**
     * This is the most flexible means of appending an element to an array of variant.
     * It behaves like the previous two va_append_element methods, but widens the variant type if and only if the
     * type of the element to be appended is not already part of that type.
     */
    template<size_t N, typename T, typename S>
    constexpr std::array<typename details::concat1_prepend<S, T>::type, N + 1>
    va_prepend_element(const S &s, const std::array<T, N> &a) noexcept {
        return details::va_prepend_element_aux(s, a, std::make_index_sequence<N>{});
    }

    /**
     * This is the most flexible means of prepending an element to an array of variant.
     * It behaves like the previous two va_prepend_element methods, but widens the variant type if and only if the
     * type of the element to be prepended is not already part of that type.
     */
    template<size_t N, typename T, typename S>
    constexpr std::array<typename details::concat1_append<S, T>::type, N + 1>
    va_append_element(const std::array<T, N> &a, const S &s) noexcept {
        return details::va_append_element_aux(a, s, std::make_index_sequence<N>{});
    }

    /// Need this to print; nothing will be printed no matter what is defined here.
    std::ostream &operator<<(std::ostream &out, const std::monostate &m) noexcept {
        return out;
    }
}