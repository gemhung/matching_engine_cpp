#include <iostream>
#include <tuple>

template <size_t... S>
constexpr auto forEachInternal(const auto& lhs, const auto& rhs, std::index_sequence<S...>) {
    return decltype(lhs){(rhs[S] * lhs[S])...};
}

constexpr auto operator*(const auto& lhs, const auto& rhs) {
    return forEachInternal(lhs, rhs, std::make_index_sequence<std::tuple_size<std::decay_t<decltype(lhs)>>::value>{});
}

template <int... I>
struct Vector {
    std::array<int, sizeof...(I)> array_{I...};
};

template <typename... Types>
struct Zip {
    std::tuple<Types...> tuple_;
};

template <typename... Types, size_t... S>
constexpr auto expandTuple(const std::tuple<Types...>& tp, std::index_sequence<S...>) {
    return (std::get<S>(tp).array_ * ...);
}

template <typename... Types>
constexpr auto getZip(const std::tuple<Types...>& tp) {
    return expandTuple(tp, std::make_index_sequence<sizeof...(Types)>{});
}

int main() {
    constexpr Vector<28, 80, 162> vec;
    constexpr Zip<Vector<1, 2, 3>, Vector<4, 5, 6>, Vector<7, 8, 9>> zip;
    constexpr auto my_zip = getZip(zip.tuple_);
    static_assert(std::is_same_v<std::decay_t<decltype(my_zip)>, std::decay_t<decltype(vec.array_)>>, "not match");
    static_assert(my_zip[0] == vec.array_[0], "!=");
    static_assert(my_zip[1] == vec.array_[1], "!=");
    static_assert(my_zip[2] == vec.array_[2], "!=");
    for (auto& z : my_zip)
        std::cout << z << std::endl;
    return 0;
}
