export module unroll;

import <type_traits>;
import <utility>;

export template <typename... Ts, typename F>
constexpr void for_types(F&& f)
{
    (f.template operator()<Ts>(), ...);
}

export template <auto... Xs, typename F>
constexpr void for_values(F&& f)
{
    (f.template operator()<Xs>(), ...);
}

export template <auto B, auto E, typename F>
constexpr void for_range(F&& f)
{
    using t = std::common_type_t<decltype(B), decltype(E)>;

    [&f]<auto... Xs>(std::integer_sequence<t, Xs...>)
    {
        for_values<(B + Xs)...>(f);
    }
    (std::make_integer_sequence<t, E - B>{});
}

template <typename... Ts, typename F>
constexpr void enumerate_types(F&& f)
{
    [&f]<auto... Is>(std::index_sequence<Is...>)
    {
        (f.template operator()<Ts, Is>(), ...);
    }(std::index_sequence_for<Ts...>{});
}
