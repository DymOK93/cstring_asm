#pragma once
#include <type_traits>
#include <utility>

namespace algo::container {
template <class Ty, class = void>
struct is_iterable : std::false_type {};

template <class Ty>
struct is_iterable<Ty,
                   std::void_t<decltype(std::declval<Ty>().begin()),
                               decltype(std::declval<Ty>().end())>>
    : std::true_type {};

template <class Ty>
inline constexpr bool is_iterable_v = is_iterable<Ty>::value;

template <class Ty>
struct range_based_for_supporting {
  static constexpr bool value{
      std::disjunction_v<std::is_array<Ty>, is_iterable<Ty>>};
};

template <class Ty>
inline constexpr bool range_based_for_supporting_v =
    range_based_for_supporting<Ty>::value;

template <class Ty, class = void>
struct has_traits : std::false_type {};

template <class Ty>
struct has_traits<Ty, std::void_t<typename Ty::traits_type>> : std::true_type {
};

template <class Ty>
inline constexpr bool has_traits_v = has_traits<Ty>::value;

template <class Ty, class = void>
struct has_value_type : std::false_type {};

template <class Ty>
struct has_value_type<Ty, std::void_t<typename Ty::value_type>>
    : std::true_type {};

template <class Ty>
inline constexpr bool has_value_type_v = has_value_type<Ty>::value;

template <class Ty, class = void>
struct has_key_type : std::false_type {};

template <class Ty>
struct has_key_type<Ty, std::void_t<typename Ty::key_type>> : std::true_type {};

template <class Ty>
inline constexpr bool has_key_type_v = has_key_type<Ty>::value;

template <class Ty, class = void>
struct has_mapped_type : std::false_type {};

template <class Ty>
struct has_mapped_type<Ty, std::void_t<typename Ty::mapped_type>>
    : std::true_type {};

template <class Ty>
inline constexpr bool has_mapped_type_v = has_mapped_type<Ty>::value;

template <class Ty>
struct is_container {
  static constexpr bool value{
      std::conjunction_v<has_value_type<Ty>, std::negation<has_traits<Ty>>>};
};

template <class Ty>
inline constexpr bool is_container_v = is_container<Ty>::value;

template <class Ty>
struct is_set {
  static constexpr bool value{
      std::conjunction_v<is_container<Ty>,
                         has_key_type<Ty>,
                         std::negation<has_mapped_type<Ty>>>};
};

template <class Ty>
inline constexpr bool is_set_v = is_set<Ty>::value;

template <class Ty>
struct is_map {
  static constexpr bool value{std::conjunction_v<is_container<Ty>,
                                                 has_key_type<Ty>,
                                                 has_mapped_type<Ty>>};
};

template <class Ty>
inline constexpr bool is_map_v = is_map<Ty>::value;

template <class Ty, class = void>
struct has_reserve : std::false_type {};

template <class Ty>
struct has_reserve<
    Ty,
    std::void_t<decltype(std::declval<Ty>().reserve(std::declval<size_t>()))>>
    : std::true_type {};

template <class Ty>
inline constexpr bool has_reserve_v = has_reserve<Ty>::value;

template <class Ty, class = void>
struct is_linear : std::false_type {};

template <class Ty>
struct is_linear<Ty,
                 std::void_t<decltype(std::declval<Ty>().push_back(
                     std::declval<typename Ty::value_type>()))>>
    : std::true_type {};

template <class Ty>
inline constexpr bool is_linear_v = is_linear<Ty>::value;

template <class Ty, class = void>
struct is_associative : std::false_type {};

template <class Ty>
struct is_associative<Ty,
                      std::void_t<decltype(std::declval<Ty>().insert(
                          std::declval<typename Ty::value_type>()))>>
    : std::true_type {};

template <class Ty>
inline constexpr bool is_associative_v = is_associative<Ty>::value;

template <class Ty, class = void>
struct data_type {
  using type = typename Ty::value_type;
};

template <class Ty>
struct data_type<Ty,
                 std::void_t<typename Ty::key_type, typename Ty::mapped_type>> {
  using type = std::pair<typename Ty::key_type, typename Ty::mapped_type>;
};

template <class Ty>
using data_type_t = typename data_type<Ty>::type;
}  // namespace algo::container
